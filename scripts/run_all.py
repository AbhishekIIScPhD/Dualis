import argparse
import subprocess
import multiprocessing
import os
import time
import sys

def run_benchmark_pipeline(mode, benchmark):
    start_time = time.time()
    pid = os.getpid()
    print(
        f"\n[PID {pid}] Starting full pipeline for benchmark: "
        f"'{benchmark}'"
    )
    
    output_directory = "../Logs/"+ mode +"Pipeline_Logs/"
    os.makedirs(output_directory, exist_ok=True)
    
    log_file_path = os.path.join(output_directory, f"{benchmark}_pipeline.log")
    
    command = [sys.executable, "chcverifynfuzz.py", mode, benchmark]
    
    try:
        with open(log_file_path, 'w') as log_file:
            log_file.write(f"Running command: {' '.join(command)}\n")
            log_file.write(f"PID: {pid}\n")
            log_file.write("="*40 + "\n\n")
            
            subprocess.run(
                command,
                check=True,
                text=True,
                stdout=log_file,
                stderr=log_file
            )
        
        end_time = time.time()
        duration = end_time - start_time
        print(
            f"[PID {pid}] Finished successfully: '{benchmark}'. "
            f"Duration: {duration:.2f}s. See log: {log_file_path}"
        )
    except subprocess.CalledProcessError:
        print(
            f"[PID {pid}] ERROR in pipeline for '{benchmark}'. "
            f"Check log for details: {log_file_path}"
        )
    except FileNotFoundError:
        print(
            f"[PID {pid}] FATAL ERROR: Could not find the worker script "
            f"'CHCVerifyNRapidCheck.py'."
        )
    except Exception as e:
        print(f"[PID {pid}] An unexpected error occurred for '{benchmark}': {e}")


def main():
    print("=" * 43)
    print(" Running Benchmarks in Parallel")
    print("=" * 43 + "\n")

    parser = argparse.ArgumentParser(
        description="Run benchmark pipelines in parallel.",
        formatter_class=argparse.RawTextHelpFormatter,
        epilog="""\
        Example usage:
        Run a single benchmark (e.g., 'Stack'):
        python3 %(prog)s ClassicalHornICE Stack

        Run all benchmarks using 4 processes:
        python3 %(prog)s ContextualLLMHornICE all -p 4
        """
    )
    parser.add_argument(
        "mode",
        metavar="MODE",
        choices=['ClassicalHornICE',
                 'ClassicalHornICEKLEE',
                 'ClassicalHornICEFUZZ',
                 'ClassicalLLMHornICE',
                 'ClassicalLLMHornICEKLEE',
                 'ClassicalLLMHornICEFUZZ',
                 'ContextualHornICE',
                 'ContextualHornICEFUZZ',
                 'ContextualLLMHornICEKLEE',
                 'ContextualLLMHornICEFUZZ',
                 'ClassicalCVC5',
                 'ContextualCVC5'
                 ],
        help="The execution mode for the pipeline."
    )
    parser.add_argument(
        "benchmark_name",
        metavar="BENCHMARK",
        help="The name of the benchmark to run, or 'all' to run all."
    )
    parser.add_argument(
        "-p", "--processes",
        type=int,
        default=1,
        help="Number of processes. -1 uses all cores. Default is 1."
    )
    args = parser.parse_args()

    benchmark_list = [ "AlternatingList", "AtomicHashMap1",
                       "AtomicHashMap2", "AtomicHashMap3", "AtomicHashMap4",
                       "AtomicHashMap5", "AtomicLinkedList1", "AtomicLinkedList2",
                       "BinaryHeap1", "BinaryHeap2", "BinaryTree", "BlueWhite",
                       "Calender", "DLL_Circular", "DLL_Token", "FlatHashMap1",
                       "FlatHashMap2", "FlatHashMap3", "FlatHashMap4", "FlatHashSet",
                       "LruCache1", "Max", "Min", "Multimap1", "Multimap2",
                       "Multiset1", "Multiset2", "NormalFilterQueue",
                       "PriorityFilterQueue", "ProcessQueue", "RedBlackTree",
                       "SkipList1", "SkipList2", "SkipList3", "SkipList4",
                       "SkipList5", "SkipList6", "SkipList7", "Stack", "StockOrder",
                       "TokenBucket1", "TokenBucket2", "TokenBucket3" ]

    if args.benchmark_name.lower() == "all":
        if args.processes == -1:
            num_processes = os.cpu_count()
        else:
            num_processes = max(1, args.processes)
            
        num_processes = min(num_processes, len(benchmark_list))
        
        print(
            f"Running all {len(benchmark_list)} benchmarks in parallel "
            f"using {num_processes} processes...\n"
        )
        
        with multiprocessing.Pool(processes=num_processes) as pool:
            tasks = [(args.mode, benchmark) for benchmark in benchmark_list]
            
            try:
                pool.starmap(run_benchmark_pipeline, tasks)
            except KeyboardInterrupt:
                print("\nCaught KeyboardInterrupt, terminating workers.")
                pool.terminate()
                pool.join()
                sys.exit(1)

        print("\nAll benchmark pipelines have completed.")
    else:
        if args.benchmark_name in benchmark_list:
            print(
                f"Running a single pipeline for benchmark: "
                f"{args.benchmark_name}\n"
            )
            run_benchmark_pipeline(args.mode, args.benchmark_name)
        else:
            print(
                f"Error: Benchmark '{args.benchmark_name}' not found in the "
                f"benchmark list."
            )

if __name__ == "__main__":
    main()
