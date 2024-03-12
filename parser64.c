#include "parser.h"


SStrings *make_sh_strtb(FILE *file, Elf64_Shdr *section_table, long count, long e_sht_idx)
{
    SStrings *strings;

    strings = malloc(sizeof(SStrings));
    strings->sh_strtb = malloc(section_table[e_sht_idx].sh_size);
    strings->strtbs = malloc(sizeof(*strings->strtbs) * count);
    strings->count = count;

    fseek(file, section_table[e_sht_idx].sh_offset, SEEK_SET);
    fread(strings->sh_strtb, sizeof(char), section_table[e_sht_idx].sh_size, file);

    for (int i = 0; i < count; ++i)
    {
        strings->strtbs[i] = NULL;
        if (!strncmp(".strtab", strings->sh_strtb + section_table[i].sh_name, 8))
        {
            strings->strtbs[i] = malloc(section_table[i].sh_size);
            fseek(file, section_table[i].sh_offset, SEEK_SET);
            fread(strings->strtbs[i], sizeof(char), section_table[i].sh_size, file);
            printf("String table \n");
            for (int n = 0; n < section_table[i].sh_size; ++n)
            {
                if (strings->strtbs[i][n] == '\0')
                    printf("\\0|");
                else
                    printf("%c|", strings->strtbs[i][n]);
            }
            printf("\n");
        }
    }
    strings->strtbs[e_sht_idx] = strings->sh_strtb;

    return strings;
}

void print_section_headers(Elf64_Shdr *section_table, SStrings *strings)
{
    printf("[Nr] %-20s %-15s %-16s %-8s %-16s %-11s %-16s %-6s\n",
           "Name", "Type", "Address", "Offset", "Size", "Entry Size", "Flags", "Align");
    for (long i = 0; i < strings->count; ++i)
    {
        printf("[%2d] ", i);
        printf("%-20s", strings->sh_strtb + section_table[i].sh_name);
        print_section_type(section_table[i].sh_type);
        printf("%016d ", section_table[i].sh_addr);
        printf("%08x ", section_table[i].sh_offset);
        printf("%016x ", section_table[i].sh_size);
        printf("%011x ", section_table[i].sh_entsize);
        print_section_flags(section_table[i].sh_flags);
        // printf("%6d ", section_table[i].sh_flags);
        printf("%6d ", section_table[i].sh_addralign);
        printf("\n");
    }
    printf("Done\n");
}

void print_symbol_type(long type)
{
    switch (ELF64_ST_TYPE(type))
    {
    case (STT_NOTYPE):
        printf("%-15s", STRING_TOKEN(STT_NOTYPE));
        break;

    case (STT_OBJECT):
        printf("%-15s", STRING_TOKEN(STT_OBJECT));
        break;

    case (STT_FUNC):
        printf("%-15s", STRING_TOKEN(STT_FUNC));
        break;

    case (STT_SECTION):
        printf("%-15s", STRING_TOKEN(STT_SECTION));
        break;

    case (STT_FILE):
        printf("%-15s", STRING_TOKEN(STT_FILE));
        break;

    case (STT_COMMON):
        printf("%-15s", STRING_TOKEN(STT_COMMON));
        break;

    case (STT_TLS):
        printf("%-15s", STRING_TOKEN(STT_TLS));
        break;

    default:
        printf("Invalid type");
        break;
    }
}

void print_symbol_bind(long bind)
{
    switch (ELF64_ST_BIND(bind))
    {
    case (STB_LOCAL):
        printf("%-15s", STRING_TOKEN(STB_LOCAL));
        break;
    case (STB_GLOBAL):
        printf("%-15s", STRING_TOKEN(STB_GLOBAL));
        break;
    case (STB_WEAK):
        printf("%-15s", STRING_TOKEN(STB_WEAK));
        break;
    default:
        printf("U_BIND");
        break;
    }
}

void print_symbol_vis(long vis)
{
    switch (vis)
    {
    case (STT_NOTYPE):
        printf("%-15s", STRING_TOKEN(STT_NOTYPE));
        break;
    case (STT_OBJECT):
        printf("%-15s", STRING_TOKEN(STT_OBJECT));
        break;
    case (STT_FUNC):
        printf("%-15s", STRING_TOKEN(STT_FUNC));
        break;
    case (STT_SECTION):
        printf("%-15s", STRING_TOKEN(STT_SECTION));
        break;
    case (STT_FILE):
        printf("%-15s", STRING_TOKEN(STT_FILE));
        break;
    case (STT_COMMON):
        printf("%-15s", STRING_TOKEN(STT_COMMON));
        break;
    case (STT_TLS):
        printf("%-15s", STRING_TOKEN(STT_TLS));
        break;
    }
}

void print_symtab(SectionOcc *tables, Section *all_sections, SStrings *strings)
{
    long entry_count;
    Elf64_Sym *symbols;
    Section *section;

    for (int x = 0; x < tables->count; ++x)
    {
        section = tables->sections[x];
        symbols = (Elf64_Sym *)section->data;
        entry_count = section->header->sh_size / section->header->sh_entsize;
        printf("Symbol table %s contains %d entries : \n", strings->sh_strtb + section->header->sh_name, entry_count);
        printf("[Nr]: %-18s %-3s %-15s %-15s %-15s %-4s %-10s\n",
            "Value", "Size", "Type", "Bind", "Vis", "Ndx", "Name");
        for (int i = 0; i < entry_count; ++i)
        {
            printf("[%2d] ", i);
            printf("%018d ", symbols[i].st_value);
            printf("%5d ", symbols[i].st_size);
            print_symbol_type(symbols[i].st_info);
            print_symbol_bind(symbols[i].st_info);
            print_symbol_vis(symbols[i].st_other);
            printf("%5d ", symbols[i].st_shndx);
            // Link is the section header index of the associated string table.
            // If the symbol is a section, the name is in the sh_strb instead of the associated string table (which is null)
            if (ELF64_ST_TYPE(symbols[i].st_info) == STT_SECTION)
            {
                printf("%-10s", strings->sh_strtb + all_sections[symbols[i].st_shndx].header->sh_name);
            }
            else
                printf("%-10s", ((char *)section->link->data) + symbols[i].st_name);
            printf("\n");
        }
    }
    printf("\n");
}

void print_relas(SectionOcc *tables, Section *all_sections, SStrings *strings)
{
    Elf64_Rela *rela;
    long entry_count;
    Section *section;
    Section *symbol;

    for (int x = 0; x < tables->count; ++x)
    {
        section = tables->sections[x];
        entry_count = section->header->sh_size / section->header->sh_entsize;
        printf("Relocation section %s at offset 0x%x contains %d entries: \n",
            strings->sh_strtb + section->header->sh_name ,
            section->header->sh_offset,
            entry_count);

        printf("%-18s %-18s %-18s %-18s %-20s\n",
        "Offset", "Info", "Type", "Sym. Value", "Sym. Name + Addend");

        rela = section->data;
        for (int i = 0;  i < entry_count; ++i)
        {
            symbol = section->link;
            Elf64_Sym *symdata = &(((Elf64_Sym *)(symbol->data))[ELF64_R_SYM(rela[i].r_info)]);
            printf("%018x ", rela[i].r_offset);
            printf("%018x ", rela[i].r_info);
            printf("%018x ", ELF64_R_TYPE(rela[i].r_info));
            printf("%018d ", symdata->st_value);

            if (ELF64_ST_TYPE(symdata->st_info) == STT_SECTION)
                printf("%10s + %d", strings->sh_strtb + all_sections[symdata->st_shndx].header->sh_name, rela[i].r_addend);
            else
                printf("%10s + %d", ((char *)(symbol->link->data)) +  symdata->st_name, rela[i].r_addend);
            printf("\n");
        }
    }
    printf("\n");
}

void print_relss(SectionOcc *tables, Section *all_sections, SStrings *strings)
{
    Elf64_Rela *rel;
    long entry_count;
    Section *section;
    Section *symbol;

    for (int x = 0; x < tables->count; ++x)
    {
        section = tables->sections[x];
        entry_count = section->header->sh_size / section->header->sh_entsize;
        printf("Relocation section %s at offset 0x%x contains %d entries: \n",
            strings->sh_strtb + section->header->sh_name ,
            section->header->sh_offset,
            entry_count);

        printf("%-18s %-18s %-18s %-18s %-20s\n",
        "Offset", "Info", "Type", "Sym. Value", "Sym. Name");

        rel = section->data;
        for (int i = 0;  i < entry_count; ++i)
        {
            symbol = section->link;
            Elf64_Sym *symdata = &(((Elf64_Sym *)(symbol->data))[ELF64_R_SYM(rel[i].r_info)]);
            printf("%018x ", rel[i].r_offset);
            printf("%018x ", rel[i].r_info);
            printf("%018x ", ELF64_R_TYPE(rel[i].r_info));
            printf("%018d ", symdata->st_value);

            if (ELF64_ST_TYPE(symdata->st_info) == STT_SECTION)
                printf("%10s", strings->sh_strtb + all_sections[symdata->st_shndx].header->sh_name);
            else
                printf("%10s", ((char *)(symbol->link->data)) +  symdata->st_name);
            printf("\n");
        }
    }
    printf("\n");
}

Section *make_sections(FILE *file, Elf64_Shdr *headers, long count)
{
    Section *result;
    result = calloc(count,  sizeof(Section));
    for (int i = 0; i < count; ++i)
    {
        result[i].data = malloc(headers[i].sh_size);
        result[i].header = &headers[i];
        int c = fseek(file, headers[i].sh_offset, SEEK_SET);
        // printf("Size %d %x\n", c, headers[i].sh_offset);
        fread(result[i].data, headers[i].sh_size, 1, file);
        result[i].link = &result[headers[i].sh_link];
    }

    return result;
}

void make_sect_tab(SectionT *sect_tab, Section *sections, long count)
{

    printf("Count %ld\n", count);
    for (int i = 0; i < count; ++i)
    {
        switch (sections[i].header->sh_type){
            case (SHT_STRTAB):
                sect_tab->strtab.count++;
                break ;
            case (SHT_SYMTAB):
                sect_tab->symtab.count++;
                break ;
            case (SHT_DYNAMIC):
                sect_tab->dyn.count++;
                break ;
            case (SHT_REL):
                sect_tab->rel.count++;
                break ;
            case (SHT_RELA):
                sect_tab->rela.count++;
                break ;
            case (SHT_NOTE):
                sect_tab->note.count++;
                break ;
            default :
                break ;
        }
    }

    sect_tab->strtab.sections = malloc(sizeof (Section *) * sect_tab->strtab.count);
    sect_tab->strtab.count = 0;
    sect_tab->symtab.sections = malloc(sizeof (Section *) * sect_tab->symtab.count);
    sect_tab->symtab.count = 0;
    sect_tab->dyn.sections = malloc(sizeof (Section *) * sect_tab->dyn.count);
    sect_tab->dyn.count = 0;
    sect_tab->rel.sections = malloc(sizeof (Section *) * sect_tab->rel.count);
    sect_tab->rel.count = 0;
    sect_tab->rela.sections = malloc(sizeof (Section *) * sect_tab->rela.count);
    sect_tab->rela.count = 0;
    sect_tab->note.sections = malloc(sizeof (Section *) * sect_tab->note.count);
    sect_tab->note.count = 0;



    for (int i = 0; i < count; ++i)
    {
        switch (sections[i].header->sh_type){
            case (SHT_STRTAB):
                sect_tab->strtab.sections[sect_tab->strtab.count] = &sections[i];
                sect_tab->strtab.count++;
                break ;
            case (SHT_SYMTAB):
                sect_tab->symtab.sections[sect_tab->symtab.count] = &sections[i];
                sect_tab->symtab.count++;
                break ;
            case (SHT_DYNAMIC):
                sect_tab->dyn.sections[sect_tab->dyn.count] = &sections[i];
                sect_tab->dyn.count++;
                break ;
            case (SHT_REL):
                sect_tab->rel.sections[sect_tab->rel.count] = &sections[i];
                sect_tab->rel.count++;
                break ;
            case (SHT_RELA):
                sect_tab->rela.sections[sect_tab->rela.count] = &sections[i];
                sect_tab->rela.count++;
                break ;
            case (SHT_NOTE):
                sect_tab->note.sections[sect_tab->note.count] = &sections[i];
                sect_tab->note.count++;
                break ;
            default :
                break ;
        }
    }

}

void read_sections64(FILE *file, Elf64_Shdr *sect_hdr, long count, Elf64_Half e_shstrndx)
{
    SStrings    *strings;
    Section     *sections;
    SectionT    sec_tab = {0};

    strings = make_sh_strtb(file, sect_hdr, count, e_shstrndx);
    sections = make_sections(file, sect_hdr, count);

    make_sect_tab(&sec_tab, sections, count);
    print_section_headers(sect_hdr, strings);

    if (sec_tab.symtab.count > 0)
        print_symtab(&sec_tab.symtab, sections,strings);
    if (sec_tab.rela.count > 0)
        print_relas(&sec_tab.rela, sections, strings);
    if (sec_tab.rel.count > 0)
        print_relas(&sec_tab.rel, sections, strings);


    // for (int i = 0; i < count; ++i)
    // {
    //     if (!strncmp(".symtab", strings->sh_strtb + sect_hdr[i].sh_name, 8))
    //         read_symtab(file, sect_hdr, &sect_hdr[i], strings);
    //     if (!strncmp(".rela", strings->sh_strtb + sect_hdr[i].sh_name, 5))
    // }
}

int read_header_info(FILE *file, unsigned char *hdr_info)
{
    fread(hdr_info, EI_NIDENT, 1, file);

// ------------------ Error check
    if (ftell(file) != EI_NIDENT)
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

int read_segments(FILE *file, Elf64_Phdr *seg_hdr, long count)
{
    printf("%-18s %-16s %-16s %-16s -16s -16s -16s -5s -8s\n",
    "Type", "Offset", "VirtAdrr", "PhysAddr", "FileSiz", "MemSiz", "Flg", "Align");
    for ( int x = 0; x < count; ++x)
    {
        printf("%16d", seg_hdr[x].p_type );
        printf("0x%12x", seg_hdr[x].p_offset );
        printf("0x%16x", seg_hdr[x].p_vaddr );
        printf("0x%16x", seg_hdr[x].p_paddr );
        printf("%16d", seg_hdr[x].p_filesz );
        printf("%4d", seg_hdr[x].p_flags );
        printf("%8d", seg_hdr[x].p_align);

    }
}

