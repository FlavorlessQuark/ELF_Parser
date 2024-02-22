#include "parser.h"

// Fill hdr_info + return obj type (32 or 64) || Not gonna handle little vs big endian
int read_header_info(int fd, unsigned char *hdr_info)
{

    int bytes_read;

    bytes_read = read(fd, hdr_info, EI_NIDENT);

// ------------------ Error check
    if (bytes_read != EI_NIDENT)
        D_ERROR("Invalid file size")

// Should start with 0x7fELF
    if (
        hdr_info[EI_MAG0] != ELFMAG0 ||
        hdr_info[EI_MAG1] != ELFMAG1 ||
        hdr_info[EI_MAG2] != ELFMAG2 ||
        hdr_info[EI_MAG3] != ELFMAG3
        )
        D_ERROR("Invalid ELF header")

    int class;

// 1: 32bit 2: 64 bit
    if ((class = hdr_info[EI_CLASS]) == ELFCLASSNONE)
        D_ERROR("Invalid ELF class")

// 1: Little endian 2: Big endian
    if (hdr_info[EI_DATA] == ELFDATANONE)
        D_ERROR("Invalid data encoding")

    if (hdr_info[EI_VERSION] != EV_CURRENT)
        D_ERROR("Invalid version")

    return class;
}
// %[flags][width][.precision][size]type

void print_header_info(unsigned char *hdr_info)
{
    printf("\nELF header: \n");

    printf("%-36s", "Magic:", "");
    for (int i = 0; i < EI_NIDENT; ++i)
        printf("%x ", hdr_info[i]);
    printf("\n");

    printf("%-36s", "Class:");
    switch (hdr_info[EI_CLASS])
    {
        case ELFCLASS32:
            printf("32 bit");
            break;
        case ELFCLASS64:
            printf("64 bit");
            break;
        default:
            printf("Invalid data somehow passed check");
            break;
    }
    printf("\n");

    printf("%-36s", "Data:");
    switch (hdr_info[EI_DATA])
    {
        case ELFDATA2LSB:
            printf("2's complement, little endian");
            break;
        case ELFDATA2MSB:
            printf("2's complement, big endian");
            break;
        default:
            printf("Invalid data somehow passed check");
            break;
    }
    printf("\n");

    printf("%-36s%d\n", "Version:", hdr_info[EI_VERSION]);

    printf("%-36s", "ABI:");
    switch (hdr_info[EI_OSABI])
    {
        case ELFOSABI_SYSV:
			printf("UNIX System V ABI\n");
			break;

		case ELFOSABI_HPUX:
			printf("HP-UX\n");
			break;

		case ELFOSABI_NETBSD:
			printf("NetBSD\n");
			break;

		case ELFOSABI_GNU:
			printf(" Object uses GNU ELF extensions.\n");
			break;

		case ELFOSABI_SOLARIS:
			printf("Sun Solaris\n");
			break;

		case ELFOSABI_AIX:
			printf("IBM AIX\n");
			break;

		case ELFOSABI_IRIX:
			printf("SGI Irix\n");
			break;

		case ELFOSABI_FREEBSD:
			printf("FreeBSD\n");
			break;

		case ELFOSABI_TRU64:
			printf("Compaq TRU64 UNIX\n");
			break;

		case ELFOSABI_MODESTO:
			printf("Novell Modesto\n");
			break;

		case ELFOSABI_OPENBSD:
			printf("OpenBSD\n");
			break;

		case ELFOSABI_ARM_AEABI:
			printf("ARM EABI\n");
			break;

		case ELFOSABI_ARM:
			printf("ARM\n");
			break;

		case ELFOSABI_STANDALONE:
			printf("Standalone (embedded) application\n");
			break;

		default:
			printf("Unknown (0x%x)\n", hdr_info[EI_OSABI]);
			break;
    }

    printf("%-36s%d\n", "ABI version:", hdr_info[EI_ABIVERSION]);
}
