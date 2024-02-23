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
    //too many machine types and not relevant anyways
    printf("%-36s", "Type:");
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

void read_header64(FILE *file, Elf64_Ehdr *header)
{
    fread(((char *)header) + EI_NIDENT, sizeof(Elf64_Ehdr) - EI_NIDENT, 1, file);

    if (ftell(file) != sizeof(Elf64_Ehdr))
        D_ERROR("Invalid file size")

    // ------------------ Print info
    print_header_info(header->e_ident);
    print_type64(header->e_type);
    // print_machine64(header->e_machine);
    printf("%-36s0x%08x\n", "Entry point: ", header->e_entry);
    printf("%-36s%d\n", "Elf header size: ", header->e_ehsize);

    printf("%-36s%d (bytes)\n", "Section header offset: ", header->e_shoff);
    printf("%-36s%d\n", "Section header size: ", header->e_shentsize);
    if ( header->e_shnum == SHN_UNDEF)
        printf("%-36s%s\n", "Section entries count:", "Undefined");
    else
        printf("%-36s%d\n", "Section entries count:", header->e_shnum);

    printf("%-36s%d (bytes)\n", "Program header offset: ", header->e_phoff);
    printf("%-36s%d\n", "Program header size: ", header->e_phentsize);
    printf("%-36s%d\n", "Program entries count:", header->e_phnum);
    // skipping flags because irrelevant
}
