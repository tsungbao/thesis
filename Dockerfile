FROM ubuntu
RUN apt-get update  && apt-get install -y g++ 
RUN apt-get update  && apt-get install -y make 
RUN apt-get update  && apt-get install -y vim
RUN apt-get update && apt-get install -y gdb
WORKDIR /thesis
COPY ./ ./
RUN make debug.o
RUN make test.o