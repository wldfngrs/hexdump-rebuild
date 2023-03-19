#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

char* helptext = "Usage: hexdump [file]\n";

void print_chars(uint8_t* buffer, size_t l) {
	for (int i = l - 16; i < l; i++) {
		if (buffer[i] > 31 && buffer[i] < 127) {
			printf("%c", buffer[i]);
		} else {
			printf(".");
		}
	}
}

void print_buffer(uint8_t* buffer, int file_size) {
	for (size_t i = 0; i < file_size; i++) {
		if (i > 0 && i % 8 == 0) {
			printf("  ");
		}
		
		if (i > 0 && (i % 16 == 0)) {
			printf("|");
			print_chars(buffer, i);
			printf("|");
			printf("\n");
		}
		
		if (i < file_size) {
			printf(" %02x", buffer[i]);
		}
	}
	
	printf("\n");
}

void dump_bytes(FILE* file, int offset) {
	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);
	
	uint8_t* buffer = (uint8_t*)malloc(file_size * sizeof(uint8_t));
	if (buffer == NULL) {
		fprintf(stderr, "Unexpected error: insufficient memory.\n");
		exit(1);
	}
	
	size_t bytes_read  = fread(buffer, sizeof(uint8_t), file_size, file);
	if (bytes_read < file_size) {
		fprintf(stderr, "Unexpected error: could not read file\n");
		exit(3);
	}
	
	print_buffer(buffer, file_size);
}

int main(int argc, char** argv) {
	FILE* file;
	if (argc == 2) {
		char* dir = argv[1];
		file = fopen(dir, "rb");
		
		if (file == NULL) {
			fprintf(stderr, "Error: cannot open the file '%s'.\n", dir);
			exit(1);
		}
	} else {
		printf("Missing argument: file to dump hex.\n%s", helptext);
		exit(2);
	}
	
	dump_bytes(file, 0); // Arguments: file, offset-to-read-from
	
	fclose(file);
}
