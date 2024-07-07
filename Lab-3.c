#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char *hexDigitToBinary(char hex)
{
    switch (hex)
    {
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'a':
        return "1010";
    case 'b':
        return "1011";
    case 'c':
        return "1100";
    case 'd':
        return "1101";
    case 'e':
        return "1110";
    case 'f':
        return "1111";
    default:
        return NULL;
    }
}
char *hexToBinary(char *hex)
{
    int hexLength = strlen(hex);
    int binaryLength = hexLength * 4;
    char *binary = (char *)malloc(binaryLength + 1);

    if (binary == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    binary[binaryLength] = '\0';
    for (int i = 0, label = 0; i < hexLength; i++)
    {
        char *digitBinary = hexDigitToBinary(hex[i]);
        if (digitBinary)
        {
            strcat(binary + label, digitBinary);
            label += 4;
        }
        else
        {
            free(binary);
            return NULL;
        }
    }
    return binary;
}
int binaryToSignedDecimal(char *binary)
{
    int result = 0;
    int sign = 1;
    if (binary[0] == '1')
    {
        sign = -1;
    }
    for (int i = 1; i < strlen(binary); i++)
    {
        if (binary[i] == '1')
        {
            result = result * 2 + 1;
        }
        else if (binary[i] == '0')
        {
            result = result * 2;
        }
        else
        {
            printf("Invalid binary input: %c\n", binary[i]);
            return 0;
        }
    }
    return result * sign;
}
int binaryToDecimal(char *binary)
{
    int decimalNumber = 0;
    for (int i = 0; i < strlen(binary); i++)
    {
        if (binary[i] == '1')
        {
            decimalNumber = (decimalNumber << 1) | 1;
        }
        else if (binary[i] == '0')
        {
            decimalNumber = (decimalNumber << 1);
        }
        else
        {
            printf("Invalid character in the binary string: %c\n", binary[i]);
            return -1;
        }
    }
    return decimalNumber;
}
char *breakCharArray(char *input, int startIndex, int endIndex)
{
    int length = strlen(input);
    if (startIndex < 0 || startIndex >= length || endIndex < 0 || endIndex >= length || startIndex > endIndex)
    {
        return NULL;
    }
    int subLength = endIndex - startIndex + 1;
    char *result = (char *)malloc(subLength + 1);

    if (result == NULL)
    {
        return NULL;
    }
    strncpy(result, input + startIndex, subLength);
    result[subLength] = '\0';
    return result;
}

int labelarr[100];
int indexarr = 0;
int pcarr = 0;
int labelstore[100];
int indexstore = 0;
int pcstore = 0;
int labelprint[100];
int indexprint = 0;

int opcodefinder(char *binary)
{
    char *opcode = breakCharArray(binary, 25, 31);
    return binaryToDecimal(opcode);
}
int funct3finder(char *binary)
{
    char *funct3 = breakCharArray(binary, 17, 19);
    return binaryToDecimal(funct3);
}
int funct7finder(char *binary)
{
    char *funct7 = breakCharArray(binary, 0, 6);
    return binaryToDecimal(funct7);
}
int funct6finder(char *binary)
{
    char *funct6 = breakCharArray(binary, 0, 5);
    return binaryToDecimal(funct6);
}
int rdfinder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int rd = (decimal << 20) >> 27;
    rd = rd & 31;
    return rd;
}
int rs1finder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int rs1 = (decimal << 12) >> 27;
    rs1 = rs1 & 31;
    return rs1;
}
int rs2finder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int rs2 = (decimal << 7) >> 27;
    rs2 = rs2 & 31;
    return rs2;
}
int i_1_immfinder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int x = (decimal >> 20);
    return (int)x;
}
int i_2_immfinder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int x = ((decimal << 6) >> 26) & 63;
    return (int)x;
}
int s_immfinder(char *binary)
{
    char *x = breakCharArray(binary, 20, 24);
    char *y = breakCharArray(binary, 0, 6);
    char *s_imm = strcat(y, x);
    return binaryToSignedDecimal(s_imm);
}
int b_immfinder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int x = (decimal >> 8) & 15;
    int y = ((decimal << 1) >> 26) & 63;
    int z = (decimal >> 7) & 1;
    int w = (decimal >> 31);
    y = y << 4;
    z = z << 10;
    w = w << 11;
    x = x;
    x = (x + y + z + w) << 1;
    return x;
}
int u_immfinder(char *binary)
{
    char *u_imm = breakCharArray(binary, 0, 19);
    // return binaryToDecimal(u_imm);
    return binaryToSignedDecimal(u_imm);
}
int j_immfinder(char *binary)
{
    int decimal = binaryToDecimal(binary);
    int x = ((decimal << 1) >> 22) & 1023;
    int y = ((decimal << 11) >> 31) & 1;
    int z = ((decimal << 12) >> 24) & 255;
    int w = ((decimal >> 31));
    y = y << 10;
    z = z << 11;
    w = w << 19;
    x = x + y + z + w;
    x = x << 1;
    return (int)x;
}

void rtype(char *binary)
{
    int funct3, funct7, rd, rs1, rs2;
    funct3 = funct7 = rd = rs1 = rs2 = 0;
    funct3 = funct3finder(binary);
    funct7 = funct7finder(binary);
    rd = rdfinder(binary);
    rs1 = rs1finder(binary);
    rs2 = rs2finder(binary);
    if (funct3 == 0 && funct7 == 0)
    {
        printf("add x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 0 && funct7 == 32)
    {
        printf("sub x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 4)
    {
        printf("xor x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 6)
    {
        printf("or x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 7)
    {
        printf("and x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 1)
    {
        printf("sll x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 5 && funct7 == 0)
    {
        printf("srl x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 5 && funct7 == 32)
    {
        printf("sra x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 2)
    {
        printf("slt x%d, x%d, x%d\n", rd, rs1, rs2);
    }
    if (funct3 == 3)
    {
        printf("sltu x%d, x%d, x%d\n", rd, rs1, rs2);
    }
}
void itype(char *binary)
{
    int opcode = opcodefinder(binary);
    int funct3, rd, rs1, imm_1;
    funct3 = rd = rs1 = imm_1 = 0;
    funct3 = funct3finder(binary);
    rd = rdfinder(binary);
    rs1 = rs1finder(binary);
    imm_1 = i_1_immfinder(binary);
    if (opcode == 19)
    {
        if (funct3 == 0)
        {
            printf("addi x%d, x%d, %d\n", rd, rs1, imm_1);
        }
        if (funct3 == 4)
        {
            printf("xori x%d, x%d, %d\n", rd, rs1, imm_1);
        }
        if (funct3 == 6)
        {
            printf("ori x%d, x%d, %d\n", rd, rs1, imm_1);
        }
        if (funct3 == 7)
        {
            printf("andi x%d, x%d, %d\n", rd, rs1, imm_1);
        }
        if (funct3 == 1)
        {
            imm_1 = i_2_immfinder(binary);
            printf("slli x%d, x%d, %d\n", rd, rs1, imm_1);
        }
        if (funct3 == 5)
        {
            int funct6 = funct6finder(binary);
            imm_1 = i_2_immfinder(binary);

            if (funct6 == 0)
            {
                printf("srli x%d, x%d, %d\n", rd, rs1, imm_1);
            }
            if (funct6 == 16)
            {
                printf("srai x%d, x%d, %d\n", rd, rs1, imm_1);
            }
        }
        if (funct3 == 2)
        {
            printf("slti x%d, x%d, %d\n", rd, rs1, imm_1);
        }
        if (funct3 == 3)
        {
            printf("sltiu x%d, x%d, %d\n", rd, rs1, imm_1);
        }
    }
    if (opcode == 3)
    {
        if (funct3 == 0)
        {
            printf("lb x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
        if (funct3 == 1)
        {
            printf("lh x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
        if (funct3 == 2)
        {
            printf("lw x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
        if (funct3 == 3)
        {
            printf("ld x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
        if (funct3 == 4)
        {
            printf("lbu x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
        if (funct3 == 5)
        {
            printf("lhu x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
        if (funct3 == 6)
        {
            printf("lwu x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
    }
    if (opcode == 103)
    {
        if (funct3 == 0)
        {
            printf("jalr x%d, %d(x%d)\n", rd, imm_1, rs1);
        }
    }
}
void stype(char *binary)
{
    int funct3, rs1, rs2, imm;
    funct3 = rs1 = rs2 = imm = 0;
    funct3 = funct3finder(binary);
    rs1 = rs1finder(binary);
    rs2 = rs2finder(binary);
    imm = s_immfinder(binary);
    if (funct3 == 0)
    {
        printf("sb x%d, %d(x%d)\n", rs2, imm, rs1);
    }
    if (funct3 == 1)
    {
        printf("sh x%d, %d(x%d)\n", rs2, imm, rs1);
    }
    if (funct3 == 2)
    {
        printf("sw x%d, %d(x%d)\n", rs2, imm, rs1);
    }
    if (funct3 == 3)
    {
        printf("sd x%d, %d(x%d)\n", rs2, imm, rs1);
    }
}
void btype(char *binary)
{
    int funct3, rs1, rs2, imm;
    funct3 = rs1 = rs2 = imm = 0;
    funct3 = funct3finder(binary);
    rs1 = rs1finder(binary);
    rs2 = rs2finder(binary);
    imm = b_immfinder(binary);
    int label = imm;
    int j = 0;
    for (int i = 0; i < indexarr; i++)
    {
        if ((pcarr + imm / 4) == labelarr[i])
        {
            j = i;
            break;
        }
        j++;
    }
    if (j == indexarr)
    {
        labelarr[indexarr] = pcarr + imm / 4;
        indexarr++;
    }
    if (funct3 == 0)
    {
        printf("beq x%d, x%d, L%d\n", rs1, rs2, j + 1);
    }
    if (funct3 == 1)
    {
        printf("bne x%d, x%d, L%d\n", rs1, rs2, j + 1);
    }
    if (funct3 == 4)
    {
        printf("blt x%d, x%d, L%d\n", rs1, rs2, j + 1);
    }
    if (funct3 == 5)
    {
        printf("bge x%d, x%d, L%d\n", rs1, rs2, j + 1);
    }
    if (funct3 == 6)
    {
        printf("bltu x%d, x%d, L%d\n", rs1, rs2, j + 1);
    }
    if (funct3 == 7)
    {
        printf("bgeu x%d, x%d, L%d\n", rs1, rs2, j + 1);
    }
}
void utype(char *binary)
{
    char hex[100];
    int imm, rd;
    imm = rd = 0;
    imm = u_immfinder(binary);
    sprintf(hex, "%x", imm);
    rd = rdfinder(binary);
    printf("lui x%d, 0x%s\n", rd, hex);
}
void jtype(char *binary)
{
    int rd, imm;
    rd = imm = 0;
    rd = rdfinder(binary);
    imm = j_immfinder(binary);
    int j = 0;
    for (int i = 0; i < indexarr; i++)
    {
        if ((pcarr + imm / 4) == labelarr[i])
        {
            j = i;
            break;
        }
        j++;
    }
    if (j == indexarr)
    {
        labelarr[indexarr] = pcarr + imm / 4;
        indexarr++;
    }
    printf("jal x%d, L%d\n", rd, j + 1);
}
void converttoinstruction(char *binary)
{
    int opcode = opcodefinder(binary);

    if (opcode == 51)
    {
        rtype(binary);
    }
    else if (opcode == 19 || opcode == 3 || opcode == 103)
    {
        itype(binary);
    }
    else if (opcode == 35)
    {
        stype(binary);
    }
    else if (opcode == 99)
    {
        btype(binary);
    }
    else if (opcode == 111)
    {
        jtype(binary);
    }
    else if (opcode == 55)
    {
        utype(binary);
    }
    else
    {
        puts("Invalid type");
    }
    return;
}
void btypelabel(char *binary)
{
    int imm = 0;
    imm = b_immfinder(binary);
    labelstore[indexstore] = pcstore + imm / 4;
    indexstore++;
}
void jtypelabel(char *binary)
{
    int imm = 0;
    imm = j_immfinder(binary);
    labelstore[indexstore] = pcstore + imm / 4;
    indexstore++;
}
void decryptinstruction(char *encryptedinstruction)
{
    char *binary = hexToBinary(encryptedinstruction);
    if (binary)
    {
        converttoinstruction(binary);
    }
    else
    {
        printf("Invalid input.\n");
    }
    free(binary);
}

int main()
{
    printf("Give the name of file: ");
    char filename[256];
    scanf("%[^\n]s", filename);
    FILE *file;
    char line[100];
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Unable to open the file.\n");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        pcstore++;
        line[strcspn(line, " ")] = '\0';
        line[strcspn(line, "\n")] = '\0';
        if (opcodefinder(hexToBinary(line)) == 99)
        {
            btypelabel(hexToBinary(line));
        }
        if (opcodefinder(hexToBinary(line)) == 111)
        {
            jtypelabel(hexToBinary(line));
        }
        else
        {
            labelstore[indexstore] = 0;
            indexstore++;
        }
    }
    
    int y = 0;
    for (int i = 0; i < 100; i++)
    {
        if (labelstore[i] != 0)
        {
            if (labelprint[labelstore[i] - 1] == 0)
            {
                y++;
                labelprint[labelstore[i] - 1] = y;
            }
        }
    }
    // for (int i = 0; i < indexstore; i++)
    // {
    //     printf("%d-%d\n",labelstore[i],labelprint[i]);
    // }
    rewind(file);
    while (fgets(line, sizeof(line), file) != NULL)
    {
        pcarr++;
        line[strcspn(line, " ")] = '\0';
        line[strcspn(line, "\n")] = '\0';
        if (labelprint[pcarr - 1] != 0)
        {
            printf("L%d: ", labelprint[pcarr - 1]);
        }
        decryptinstruction(line);
    }
    fclose(file);
    return 0;
}
