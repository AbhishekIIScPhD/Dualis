import re
import os
import sys

finalSpecs = []

def _parse_specs(file_path):
    try:
        with open(file_path, 'r') as f:
            lines = f.readlines()
        
        if not lines or not lines[0].strip().isdigit():
            print(f"   -> ERROR: GenSpec.txt is empty or has invalid format.")
            return []

        num_relations = int(lines[0].strip())
        specs = []
        line_index = 1
        for _ in range(num_relations):
            if line_index + 2 >= len(lines): break
            rel = lines[line_index].strip()
            rel_decl = lines[line_index + 1].strip()
            rel_body = lines[line_index + 2].strip().split('->')[-1].strip()
            specs.append((rel, rel_decl, rel_body))
            line_index += 3
        return specs
    except FileNotFoundError:
        print(f"   -> ERROR: GenSpec.txt not found at '{file_path}'.")
        return []
    except Exception as e:
        print(f"   -> ERROR: Failed to parse GenSpec.txt: {e}")
        return []

def _parse_cvc5_specs(file_path):
    try:
        with open(file_path, 'r') as f:
             content = f.read()

        specs = []
        # Use a simple regex to find the start of each function definition
        header_pattern = re.compile(
            r'\(\s*define-fun\s+([^\s]+)\s+\((.*?)\)\s+Bool', re.DOTALL
         )

        for match in header_pattern.finditer(content):
            rel_name = match.group(1)
            params_str = match.group(2)

            # Find the start of the body, right after the full match
            body_start_index = match.end()
            body_with_trailing_parens = content[body_start_index:].strip()

            open_parens = 0
            body_end_index = -1

            # Handle simple bodies like 'true' or 'false'
            if not body_with_trailing_parens.startswith('('):
                end_paren_pos = body_with_trailing_parens.find(')')
                rel_body = body_with_trailing_parens[:end_paren_pos].strip()
            else: # Handle S-expression bodies by balancing parentheses
                for i, char in enumerate(body_with_trailing_parens):
                    if char == '(':
                        open_parens += 1
                    elif char == ')':
                        open_parens -= 1

                    if open_parens == 0:
                        body_end_index = i + 1
                        break

                if body_end_index != -1:
                    rel_body = body_with_trailing_parens[:body_end_index]
                else:
                    print(f"   -> WARNING: Could not parse body for function '{rel_name}'. Malformed S-expression.")
                    continue

            rel_decl = f"(declare-fun {rel_name} ({params_str}) Bool)"
            specs.append((rel_name, rel_decl, rel_body))
        return specs
    except Exception as e:
        print(f"   -> ERROR: Failed to parse CVC5 specs from '{file_path}': {e}")
        return []

def _parse_attributes_file(file_path):
    if not os.path.exists(file_path):
        print(f"      -> WARNING: Attributes.txt not found at '{file_path}'.")
        return {}

    mappings = {}
    try:
        with open(file_path, 'r') as f:
            lines = [line for line in f if line.strip() and not line.strip().startswith('#')]

        i = 0
        if lines and lines[0].strip().isdigit() and len(lines[0].strip().split()) == 1:
            i = 1

        while i < len(lines):
            header = lines[i].strip().split()
            relation, num_primary, num_derived = header[0], int(header[1]), int(header[2])
            i += 1

            index_map = {}
            for j in range(num_primary):
                idx, var = lines[i+j].strip().split()
                index_map[int(idx)] = var

            mappings[relation] = index_map
            i += num_primary + num_derived
        return mappings
    except Exception as e:
        print(f"      -> FATAL ERROR: Could not parse Attributes.txt: {e}")
        return {}

def apply_mapping_to_hornice_specs(specs, mappings):
    rewritten_specs = []
    for relation_name, declaration, rule in specs:
        if relation_name in mappings:
            index_map = mappings[relation_name]
            for index, variable in index_map.items():
                rule = re.sub(rf'\|{relation_name}#{index}\|', variable, rule)
        rewritten_specs.append((relation_name, declaration, rule))
    return rewritten_specs

def tokenize_formula(formula):
    tokens = []
    current_token = []
    for char in formula:
        if char in "\n":
            continue
        if char in "()" and current_token:
            tokens.append("".join(current_token))
            current_token = []
        if char in "()":
            tokens.append(char)
        elif char.isspace():
            if current_token:
                tokens.append("".join(current_token))
                current_token = []
        else:
            current_token.append(char)
    if current_token:
        tokens.append("".join(current_token))
    return tokens

def parse_let(tokens):
    assert tokens.pop(0) == "let"
    aliases = {}
    assert tokens.pop(0) == "("
    while tokens[0] != ")":
        assert tokens.pop(0) == "("
        alias = tokens.pop(0)
        value_tokens = []
        depth = 1
        while depth > 0:
            token = tokens.pop(0)
            if token == "(": depth += 1
            elif token == ")": depth -= 1
            value_tokens.append(token)
        value_tokens.pop()
        aliases[alias] = value_tokens
    assert tokens.pop(0) == ")"
    body_tokens = []
    depth = 1
    assert tokens.pop(0) == "("
    while depth > 0:
        token = tokens.pop(0)
        if token == "(": depth += 1
        elif token == ")": depth -= 1
        body_tokens.append(token)
    body_tokens.pop()
    return aliases, body_tokens

def replace_aliases(tokens, aliases):
    replaced = []
    for token in tokens:
        if token in aliases:
            replaced.extend(aliases[token])
        else:
            replaced.append(token)
    return replaced

def inorder_rewrite(tokens):
    def parse(tokens):
        token = tokens.pop(0)
        if token == "(":
            operator = tokens.pop(0)
            if operator in {"and", "or"}:
                operands = []
                while tokens[0] != ")":
                    operands.append(parse(tokens))
                tokens.pop(0)
                op_str = " && " if operator == "and" else " || "
                return f"({op_str.join(operands)})"
            elif operator == "=>":
                left = parse(tokens)
                right = parse(tokens)
                tokens.pop(0)
                return f"(!({left}) || ({right}))"
            if operator in {"+"}:
                operands = []
                while tokens[0] != ")":
                    operands.append(parse(tokens))
                tokens.pop(0)
                return f"({' + '.join(operands)})"
            if operator in {"=", "<=", ">", "<", "distinct", ">=", "*", "mod"}:
                left = parse(tokens)
                right = parse(tokens)
                tokens.pop(0) 
                return f"({left} {operator} {right})"
            elif operator in {"-", "not"}:
                left = parse(tokens)
                if tokens[0] == ")":
                    tokens.pop(0)
                    op_str = "!" if operator == "not" else "-"
                    return f"({op_str}{left})"
                else:
                    right = parse(tokens)
                    tokens.pop(0)
                    return f"({left} - {right})"
            else:
                raise ValueError(f"Unknown operator: {operator}")
        elif token == ")":
            raise ValueError("Unexpected closing parenthesis.")
        else:
            return token 
    tokens = tokens.copy()
    return parse(tokens)

def rewriteSpecs(formula):
    tokens = tokenize_formula(formula)
    if not tokens: return ""
    if tokens[0] in ["false", "true"]:
        return f"({tokens[0]})"
    
    assert tokens.pop(0) == "("
    while "let" in tokens:
        aliases, body_tokens = parse_let(tokens)
        tokens = replace_aliases(body_tokens, aliases)
    
    replaced_body = ['('] + tokens + [')']
    inorder_result = inorder_rewrite(replaced_body)
    return inorder_result
    
def formatSpecs(BenchMarkPath, solver):
    global finalSpecs
    finalSpecs.clear()

    gen_spec_file = os.path.join(BenchMarkPath, "GenSpec.txt")
    if not os.path.exists(gen_spec_file) or os.stat(gen_spec_file).st_size == 0:
        print("   -> GenSpec.txt file is empty or not found. Cannot process specs.")
        return

    benchmark_name = os.path.basename(BenchMarkPath)
    base_benchmarks_dir = os.path.abspath(os.path.join(BenchMarkPath, "../../"))
    
    contextual_path = os.path.join(base_benchmarks_dir, "Contextual", benchmark_name, "Attributes.txt")
    classical_path = os.path.join(base_benchmarks_dir, "Classical", benchmark_name, "Attributes.txt")
    
    attributes_file_path = ""
    if os.path.exists(contextual_path):
        attributes_file_path = contextual_path
    elif os.path.exists(classical_path):
        attributes_file_path = classical_path

    # specs = _parse_specs(gen_spec_file)
    # mappings = _parse_attributes_file(attributes_file_path)

    if solver == "hornice":
        specs = _parse_specs(gen_spec_file)
        mappings = _parse_attributes_file(attributes_file_path)
        rewritten_specs = apply_mapping_to_hornice_specs(specs, mappings)
    elif solver == "cvc5":
        specs = _parse_cvc5_specs(gen_spec_file)
        rewritten_specs = specs
    else:
        # Assuming hornspec logic might be needed later
        rewritten_specs = specs
        print(f"   -> ERROR: Unknown solver '{solver}' in ProcessSpecs.")
        return

    processed_specs = {}
    for relName, declaration, rule in rewritten_specs:
        if relName != "fail@":
            formattedSpec = rewriteSpecs(rule)
            formattedSpec = formattedSpec.replace(" = ", " == ")
            formattedSpec = formattedSpec.replace(" distinct ", " != ")
            formattedSpec = formattedSpec.replace(" mod ", " % ")
            processed_specs[relName] = (relName, declaration, formattedSpec)
    
    finalSpecs.extend(processed_specs.values())
    
    rewritten_specs_file = os.path.join(BenchMarkPath, 'RewrittenSpecs.txt')
    with open(rewritten_specs_file, 'w+') as f:
        for relation_name, declaration, rule in finalSpecs:
            f.write(f'{relation_name}\n{declaration}\n{rule}\n\n')

    return finalSpecs
