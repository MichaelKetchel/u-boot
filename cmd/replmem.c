// SPDX-License-Identifier: GPL-2.0+
/*
 * cmd_fs_uuid.c -- fsuuid command
 *
 * Copyright (C) 2014, Bachmann electronic GmbH
 */

#include <common.h>
#include <command.h>
// #include <fs.h>

struct cmd_tbl;


void print_memory_contents(const char *memory_ptr, size_t size) {
    // Create a temporary pointer to avoid modifying memory_ptr
    const char *temp_ptr = memory_ptr;

    // Print memory contents
    printf("Memory contents:\n");
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", (unsigned char)temp_ptr[i]);
    }
    printf("\n");
}

// Function to find and replace a given expression in memory
size_t find_and_replace(char *buffer, size_t size, const char *find, const char *replace) {
    size_t find_len = strlen(find);
    size_t replace_len = strlen(replace);
    size_t new_size = size;

    char *pos = buffer;
    while ((pos = strstr(pos, find)) != NULL) {
        memmove(pos + replace_len, pos + find_len, new_size - (pos - buffer) - find_len);
        memcpy(pos, replace, replace_len);
        pos += replace_len;
        new_size += (replace_len - find_len);
    }

    return new_size;
}


static int do_replmem(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
    // struct cmd_tbl *cp;

    if (argc < 5)
            return CMD_RET_USAGE;

	
	// Extract arguments
    const char *memory_address_str = argv[1];
	const char *memory_length_str = argv[2];
    const char *find_str = argv[3];
    const char *replace_str = argv[4];

	// char *endptr;

	
	// Convert memory address string to a pointer
    unsigned long memory_address;
    if (strict_strtoul(memory_address_str, 16, &memory_address)) {
        printf("Invalid memory address: %s\n", memory_address_str);
        return CMD_RET_FAILURE;
    }

	// Calculate buffer size (for illustration purposes, you may need to obtain this differently)
    size_t buffer_size = simple_strtoull(memory_length_str, NULL, 16);

	// Convert memory address to a char pointer
    char *memory_ptr = (char *)memory_address;

	if (argc != 6){
		printf("Old buffer size: %zu\n", buffer_size);
		print_memory_contents(memory_ptr, buffer_size);
	}
    // printf("Old buffer: %s\n", memory_ptr);

	// Find and replace the expression in memory
    size_t new_size = find_and_replace(memory_ptr, buffer_size, find_str, replace_str);

	if (argc == 6)
		env_set_hex(argv[5], new_size);
	else {
		printf("New buffer size: %zu\n", new_size);
		// printf("Modified buffer: %s\n", memory_ptr);
		print_memory_contents(memory_ptr, new_size);
	}

	
	return CMD_RET_SUCCESS;

    /* drop sub-command argument */
    // argc--;
    // argv++;

    // cp = find_cmd_tbl(argv[0], cmd_ut_sub, ARRAY_SIZE(cmd_sub));

    // if (cp)
    //     return cp->cmd(cmdtp, flag, argc, argv);

    return CMD_RET_USAGE;
}


static int do_repl_mem_wrapper(struct cmd_tbl *cmdtp, int flag, int argc,
			      char *const argv[])
{
	return do_replmem(cmdtp, flag, argc, argv);
}

U_BOOT_CMD(
	replmem, 6, 1, do_repl_mem_wrapper,
	"Finds and replaces a string in memory",
	"<address> <length> <target> <replacement>\n"
	"    - look at address, find <find> and replace with <replacement>.\n"
	"<address> <length> <target> <replacement> [new_size]\n"
	"    - look at address, find <find> and replace with <replacement>. Will set new_size with new filesize \n"
);

