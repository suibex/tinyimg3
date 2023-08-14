#!/opt/homebrew/bin/python3.10
import os,io,sys

#PREFIXED = "qemu-system-arm  -M versatilepb -cpu arm1176 -m 256 -kernel iboot  -nographic -serial telnet:localhost:4231,server,nowait,logfile=uart1.log,signal=off"
prefixed = "qemu-system-arm  -M versatilepb -cpu arm1176 -m 256 -kernel iboot -serial telnet:localhost:4321,server,nowait,logfile=uart1.log"

PREFIXED_DBG = "qemu-system-arm -s -S -M versatilepb -cpu arm1176 -m 256 -kernel iboot  -serial telnet:localhost:4321,server,nowait,logfile=uart1.log"



os.system("./xmd.sh")
if(len(sys.argv) >1 ):
  if(sys.argv[1] == "-e"):
    os.system(prefixed)

  if(sys.argv[1] == "-d"):
    os.system(PREFIXED_DBG)


