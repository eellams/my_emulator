
FILE_TO_READ = 'hello.asm'

contents = [] # What the file is read into
tokens = []
binary = ''
binHeader = []

tokenCount = 0

def Make2sCompliment(value, bits):
    ovalue = value
    if (value & (1 << (bits -  1))) != 0: # If the top bit is 1
        value = 0
        for i in xrange(bits):
            value |= (1 << i)
        value += ovalue

    print "Binary of: ", ovalue, " to ", bits , " bits is: ", value, " [" + bin(value) + "]"
    return value

# Open the file
with open(FILE_TO_READ) as f:
    contents = f.readlines()

# Deserialise into tokens
for i in xrange(len(contents)):
    try:
        contents[i] = contents[i][0:contents[i].index(';')]
    except ValueError:
        #print "No inline comment in line: ", i
        pass

    contents[i] = contents[i].strip()

    # Note, removes both spaces and commas
    tokens.append(contents[i].replace(',','').split(' '))

# Remove any empty lines
tokens = filter(lambda a: a != [''], tokens)

for i in xrange(len(tokens)):
    print tokens[i]

    if tokens[i][0] == '>':
        tokens[i][1] = int(tokens[i][1])

        while (tokens[i][1] >= len(binHeader)):
            binHeader.append(chr(0))

        print "Adding to memory header, address: ", tokens[i][1], ", value: ", tokens[i][2]
        binHeader[tokens[i][1]] = chr(int(tokens[i][2]))

        tokenCount += 1

    else:
        if tokens[i][0] == 'FINISH':
            binary = binary + str(chr(0xe0))
        elif tokens[i][0] == 'JMPI' or tokens[i][0] == 'BZ':
            tokens[i][0] = tokens[i][0]
            tokens[i][1] = int(tokens[i][1])

            if tokens[i][0] == 'JMPI':
                binary = binary + str(chr(0xa0 + Make2sCompliment(tokens[i][1], 5) ))
            elif tokens[i][0] == 'BZ':
                binary = binary + str(chr(0xc0 + Make2sCompliment(tokens[i][1], 5) ))


        else:
            tokens[i][0] = tokens[i][0]
            tokens[i][1] = int(tokens[i][1])
            tokens[i][2] = int(tokens[i][2])

            if(tokens[i][0] == 'ADD' or
            tokens[i][0] == 'LOAD' or
            tokens[i][0] == 'STORE' or
            tokens[i][0] == 'NAND'):
                print "Line ", i

            if tokens[i][0] == 'ADD':
                binary = binary + str(chr(0x20 + (tokens[i][1] << 2) + tokens[i][2] ))
            elif tokens[i][0] == 'LOAD':
                binary = binary + str(chr(0x40 + (tokens[i][1] << 2) + tokens[i][2] ))
            elif tokens[i][0] == 'STORE':
                binary = binary + str(chr(0x60 + (tokens[i][1] << 2) + tokens[i][2] ))
            elif tokens[i][0] == 'NAND':
                binary = binary + str(chr(0x80 + (tokens[i][1] << 2) + tokens[i][2] ))

            elif tokens[i][0] == 'ADDI':
                binary = binary + str(chr(0x00 + (Make2sCompliment(tokens[i][1], 3) << 2) + tokens[i][2] ))

# Add header to program
for i in xrange(len(binHeader)):
    binary = binHeader[i] + binary

# Ensure 1st byte jumps to where the actual instructions are
binary = str(chr(0xa0 + Make2sCompliment(len(binHeader)+1, 5) )) + binary[1:]

print
print "Contents:"
print contents

print
print "Tokens:"
print tokens

print
print 'Binary:'
for i in xrange(len(binary)):
    if (i < len(binHeader)):
        print i, ": ", binary[i], ' : ', hex(ord(binary[i])), ' : ', bin(ord(binary[i]))
    else:
        print i, ": ", binary[i], ' : ', hex(ord(binary[i])), ' : ', bin(ord(binary[i])), " : ", tokens[tokenCount]
        tokenCount += 1
