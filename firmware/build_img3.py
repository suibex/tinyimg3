import os,io,sys,struct
from elftools.elf.elffile import ELFFile



    
class Img3File(object):
    
    def __init__(self,fullSize,sizeNoPack,sigCheck,ident,data) -> None:
        
        self.magic = "Img3".encode("ASCII")

        self.fullSize = struct.pack("I",fullSize)
        
        self.sizeNoPack = struct.pack("I",sizeNoPack)

        self.sigCheck = struct.pack("I",sigCheck)

        self.ident = struct.pack("I",ident) #will be used an address

        self.data = data 

    def __call__(self):

        bd = self.magic+self.fullSize+self.sizeNoPack+self.sigCheck+self.ident+self.data
    
        return bd


os.system("./compile.sh")
st = open("./firm.o","rb")
file = ELFFile(st)
assert(len(sys.argv) > 1)
firmware_start_addr = int(sys.argv[1],16)
text_sect = file.get_section_by_name(".text")
assert(text_sect != None)
data  = text_sect.data()

#TODO: add SHA1 checksum 
 
img3 = Img3File((4+4+4+4+4)+len(data)+20,len(data)+20,len(data),firmware_start_addr,data)
fw= img3()

fd = open("./firmware.bin","wb")
fd.write(fw)
fd.close()

print("** [ OK ] image built\n** [INFO] firmware_start() at address",sys.argv[1],".")

