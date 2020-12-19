FROM ubuntu:20.04

RUN apt-get update && apt-get -y install libsdl2-2.0

RUN useradd -d /home/chip8 -m -s /bin/bash chip8

WORKDIR /home/chip8

COPY KAF2020_CHIP_8_nodebug KAF2020_CHIP_8
COPY solve/flag flag

COPY ynetd/ynetd .

RUN chown -R root:root /home/chip8

# USER chip8
EXPOSE 1024

CMD ./ynetd "su -c ./KAF2020_CHIP_8 chip8" -lt 3
