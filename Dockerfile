FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y build-essential cppcheck clang-tidy clang
RUN mkdir /root/advent
WORKDIR /root/advent
CMD /bin/bash