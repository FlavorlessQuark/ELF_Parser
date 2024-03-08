#pragma once

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <elf.h>


#include "debug.h"

#define STRING_TOKEN(a) (#a)


typedef struct SStrings {
    char        *sh_strtb;
    char        **strtbs;
    long        count;
}       SStrings;

typedef struct Section {
    Elf64_Shdr *header;
    struct Section *link;
    void *data;
}           Section;

typedef struct SectionOcc {
    Section **sections;
    long count;
}   SectionOcc;

typedef struct SectionT
{
    SectionOcc strtab;
    SectionOcc symtab;
    SectionOcc dyn;
    SectionOcc rel;
    SectionOcc rela;
    SectionOcc note;
}   SectionT;

int read_header_info(FILE *file, unsigned char *hdr_info);

void print_header_info(unsigned char *hdr_info);
void print_type(char c);
void print_machine(char c);
void print_flags(char c);

void print_type64(Elf64_Half c);
void print_machine64(Elf64_Half c);
void print_section_type(Elf64_Word type);
void print_section_flags(Elf64_Xword flag);
void read_header64(FILE *file, Elf64_Ehdr *header);

void read_header32(FILE *file, Elf32_Ehdr *header);
void read_header64(FILE *file, Elf64_Ehdr *header);

void read_sections64(FILE *file, Elf64_Shdr *section_table, long count, Elf64_Half e_shstrndx);
