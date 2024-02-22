#include "parser.h"


void print_type64(Elf64_Half c)
{
    printf("%-36s", "Type:");

    switch(c)
	{
		case ET_NONE:
			printf("No file type\n");
			break;

		case ET_REL:
			printf("Relocatable\n");
			break;

		case ET_EXEC:
			printf("Executable\n");
			break;

		case ET_DYN:
			printf("Shared Object\n");
			break;
		default:
			printf("Other (0x%x)\n", c);
			break;
	}
}

void print_machine64(Elf64_Half c)
{
    switch(c)
	{
		case EM_NONE:
			printf("None (0x0)\n");
			break;

		case EM_386:
			printf("INTEL x86 (0x%x)\n", EM_386);
			break;

		case EM_ARM:
			printf("ARM (0x%x)\n", EM_ARM);
			break;

		default:
			printf(" 0x%x\n",c);
			break;
	}
}

void read_header64(int fd, Elf64_Ehdr *header)
{
    int bytes_read;

    bytes_read = read(fd, header + EI_NIDENT, sizeof(Elf32_Ehdr) - EI_NIDENT);

    if (bytes_read != (sizeof(Elf32_Ehdr) - EI_NIDENT))
        D_ERROR("Invalid file size")

    // ------------------ Print info
    print_header_info(header->e_ident);
    print_type64(header->e_type);
    print_machine64(header->e_machine);
    printf("%-36s0x%08x\n", "Entry point: ", header->e_entry);
    printf("%-36s%d\n", "Program Header offset: ", header->e_phoff);
    printf("%-36s%d\n", "Section Header offset: ", header->e_shoff);
    // skipping flags because irrelevant
    printf("%-36s%d\n", "Elf header size: ", header->e_ehsize);


}
