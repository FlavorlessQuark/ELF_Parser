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

int read_header_info(FILE *file, unsigned char *hdr_info);

void print_header_info(unsigned char *hdr_info);
void print_type(char c);
void print_machine(char c);
void print_flags(char c);

void read_header32(FILE *file, Elf32_Ehdr *header);
void read_header64(FILE *file, Elf64_Ehdr *header);

void read_sections64(FILE *file, Elf64_Shdr *section_table, long count, Elf64_Half e_shstrndx);
