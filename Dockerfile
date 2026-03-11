FROM seahorn/seahorn-llvm14:nightly AS seahorn_base

FROM aflplusplus/aflplusplus:latest

USER root

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    g++ \
    build-essential \
    wget \
    && rm -rf /var/lib/apt/lists/*

COPY --from=seahorn_base /seahorn /seahorn
ENV PATH="/seahorn/build/run/bin:${PATH}"

RUN wget https://github.com/cvc5/cvc5/releases/latest/download/cvc5-Linux -O /usr/local/bin/cvc5 \
    && chmod +x /usr/local/bin/cvc5

COPY bin/ /usr/local/bin/
RUN chmod +x /usr/local/bin/chc_verifier*

COPY benchmarks /app/benchmarks
COPY Logs /app/Logs
COPY scripts /app/scripts

RUN chmod +x /app/scripts/*.py

WORKDIR /app/scripts
