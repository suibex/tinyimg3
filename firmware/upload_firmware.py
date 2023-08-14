import os,io,sys
import telnetlib
import struct


firmware = open(sys.argv[1],"rb").read()
#print(len(firmware))


tn = telnetlib.Telnet("localhost","4321")

firm_size = hex(len(firmware))[2:]
vd = ""
for _ in range (8 - len(firm_size)):
    vd+="0"

vd+=firm_size
firm_size = vd 

chunks = [] 

d = 0 

while(1):
  
  if(d >= len(firm_size)):
    break

  chunk = firm_size[d:d+2]
  print(chunk)
  chk = struct.pack("B",int(chunk,16))

  chunks.append(chk)

  d+=2


tn = telnetlib.Telnet("localhost","4321")

if(len(chunks ) > 4):

  chunks = chunks[1:]

for chunk in chunks:
  print(chunk)

  tn.write(chunk)
  #out = tn.read_all()
  #print(out)

#check for sync

print("** wait for sync...")
while(1):

  by = tn.get_socket().recv(1)
  print(by)
  if(by == b"\xf3"):
 
    print("*** invalid firmware size.")
    exit(1)

  if(by == b"\x34"):
    print("*** uart received data ")
    break

print("*** uploading firmware...")

for lin in firmware:
  tn.write(struct.pack("B",lin))

print("*** firmware uploaded.")

tn.close()



