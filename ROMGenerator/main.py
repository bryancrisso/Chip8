ROM_FNAME = "test2.ch8"

instructions = ["00E0","00EE","0000","1000","2000","3000","4000","5000","6000","7000",
                "8000","8001","8002","8003","8004","8005","8006","8007","800E","9000",
                "A000","B000","C000","D000","E09E","E0A1","F007","F00A","F015","F018",
                "F01E","F029","F033","F055","F065"]

with open(ROM_FNAME, 'wb') as f:
    for ins in instructions:
        f.write(bytes.fromhex(ins))