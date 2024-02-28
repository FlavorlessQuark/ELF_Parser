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

void print_section_type(Elf64_Word type)
{
    switch(type)
    {
        case SHT_NULL:
            printf("%-15s ", STRING_TOKEN(SHT_NULL));
            break ;
         case SHT_PROGBITS :
            printf("%-15s ", STRING_TOKEN(SHT_PROGBITS));
            break ;
        case SHT_SYMTAB :
            printf("%-15s ", STRING_TOKEN(SHT_SYMTAB));
            break ;
        case SHT_STRTAB :
            printf("%-15s ", STRING_TOKEN(SHT_STRTAB));
            break ;
        case SHT_RELA :
            printf("%-15s ", STRING_TOKEN(SHT_RELA));
            break ;
        case SHT_HASH :
            printf("%-15s ", STRING_TOKEN(SHT_HASH));
            break ;
        case SHT_DYNAMIC :
            printf("%-15s ", STRING_TOKEN(SHT_DYNAMIC));
            break ;
        case SHT_NOTE :
            printf("%-15s ", STRING_TOKEN(SHT_NOTE));
            break ;
        case SHT_NOBITS :
            printf("%-15s ", STRING_TOKEN(SHT_NOBITS));
            break ;
        case SHT_REL :
            printf("%-15s ", STRING_TOKEN(SHT_REL));
            break ;
        case SHT_SHLIB :
            printf("%-15s  ", STRING_TOKEN(SHT_SHLIB));
            break ;
        case SHT_DYNSYM :
            printf("%-15s ", STRING_TOKEN(SHT_DYNSYM));
            break ;
        case SHT_INIT_ARRAY :
            printf("%-15s ", STRING_TOKEN(SHT_INIT_ARRAY));
            break ;
        case SHT_FINI_ARRAY :
            printf("%-15s ", STRING_TOKEN(SHT_FINI_ARRAY));
            break ;
        case SHT_PREINIT_ARRAY :
            printf("%-15s ", STRING_TOKEN(SHT_PREINIT_ARRAY));
            break ;
        case SHT_GROUP :
            printf("%-15s ", STRING_TOKEN(SHT_GROUP));
            break ;
        case SHT_SYMTAB_SHNDX :
            printf("%-15s ", STRING_TOKEN(SHT_SYMTAB_SHNDX));
            break ;
        case SHT_RELR :
            printf("%-15s ", STRING_TOKEN(SHT_RELR));
            break ;
        default:
            printf("Invalid type  ");
            break ;
    }
}

void print_section_flags(Elf64_Xword flag)
{
    switch (flag)
    {
        case SHF_WRITE :
            printf("%-15s", STRING_TOKEN(SHF_WRITE));
            break ;
        case SHF_ALLOC :
            printf("%-15s", STRING_TOKEN(SHF_ALLOC));
            break ;
        case SHF_EXECINSTR :
            printf("%-15s", STRING_TOKEN(SHF_EXECINSTR));
            break ;
        case SHF_MERGE :
            printf("%-15s", STRING_TOKEN(SHF_MERGE));
            break ;
        case SHF_STRINGS :
            printf("%-15s", STRING_TOKEN(SHF_STRINGS));
            break ;
        case SHF_INFO_LINK :
            printf("%-15s", STRING_TOKEN(SHF_INFO_LINK));
            break ;
        case SHF_LINK_ORDER :
            printf("%-15s", STRING_TOKEN(SHF_LINK_ORDER));
            break ;
        case SHF_OS_NONCONFORMING :
            printf("%-15s", STRING_TOKEN(SHF_OS_NONCONFORMING));
            break ;
        case SHF_GROUP :
            printf("%-15s", STRING_TOKEN(SHF_GROUP));
            break ;
        case SHF_TLS :
            printf("%-15s", STRING_TOKEN(SHF_TLS));
            break ;
        case SHF_COMPRESSED :
            printf("%-15s", STRING_TOKEN(SHF_COMPRESSED));
            break ;
        case SHF_MASKOS :
            printf("%-15s", STRING_TOKEN(SHF_MASKOS));
            break ;
        case SHF_MASKPROC :
            printf("%-15s", STRING_TOKEN(SHF_MASKPROC));
            break ;
        case SHF_GNU_RETAIN :
            printf("%-15s", STRING_TOKEN(SHF_GNU_RETAIN));
            break ;
        case SHF_ORDERED :
            printf("%-15s", STRING_TOKEN(SHF_ORDERED));
            break ;
        case SHF_EXCLUDE :
            printf("%-15s", STRING_TOKEN(SHF_EXCLUDE));
            break ;
        default:
            printf("%-15d", flag);
            break ;
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

char * make_sh_strtb(FILE *file, Elf64_Shdr *section_table)
{
    char *table;

    table = malloc(section_table->sh_size);

    fseek(file, section_table->sh_offset, SEEK_SET);
    fread(table, sizeof(char), section_table->sh_size, file);

    return table;
}

void read_sections64(FILE *file, Elf64_Shdr *section_table, long count, Elf64_Half e_shstrndx)
{

    char *sh_strt;

    sh_strt = make_sh_strtb(file, &section_table[e_shstrndx]);

    printf("[Nr] %-20s %-15s %-16s %-8s %-16s %-11s %-16s %-6s\n",
        "Name", "Type", "Address", "Offset", "Size", "Entry Size", "Flags", "Align"
    );
    for (long i = 0; i < count; ++i)
    {
        printf("[%2d] ", i);
        printf("%-20s", sh_strt + section_table[i].sh_name);
        print_section_type(section_table[i].sh_type);
        printf("%016d ", section_table[i].sh_addr);
        printf("%08x ", section_table[i].sh_offset);
        printf("%016x ", section_table[i].sh_size);
        printf("%011d ", section_table[i].sh_entsize);
        print_section_flags(section_table[i].sh_flags);
        // printf("%6d ", section_table[i].sh_flags);
        printf("%6d ", section_table[i].sh_addralign);
        printf("\n");
    }
}
