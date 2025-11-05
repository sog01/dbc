#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {
	for (int i = 0; i < dbhdr->count; i++) {
		printf("\tName: %s\n", employees[i].name);
		printf("\tAddress: %s\n", employees[i].address);
		printf("\tHours: %d\n", employees[i].hours);
	}
}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {
	char *name = strtok(addstring, ",");
	
	char *addr = strtok(NULL, ",");
	
	char *hours = strtok(NULL, ",");
	
	printf("name = %s, addr = %s, hours = %s\n", name, addr, hours);

	strncpy(employees[dbhdr->count-1].name, name, sizeof(employees[dbhdr->count-1].name));

	strncpy(employees[dbhdr->count-1].address, addr, sizeof(employees[dbhdr->count-1].address));

	employees[dbhdr->count-1].hours = atoi(hours);	

	return STATUS_SUCCESS;
}

int update_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *updatestring) {
	char *searchname = strtok(updatestring, ":");
	
	char *name = strtok(NULL, ",");

	char *addr = strtok(NULL, ",");

	char *hours = strtok(NULL, ",");

	printf("searchname = %s, name = %s, addr = %s, hours = %s\n", searchname, name, addr, hours);

	for (int i = 0; i < dbhdr->count; i++) {
		if(strncmp(employees[i].name, searchname, sizeof(employees[i].name)) == 0) {
			strncpy(employees[i].name, name, sizeof(employees[i].name));
			strncpy(employees[i].address, addr, sizeof(employees[i].address));
			employees[i].hours = atoi(hours);
			return STATUS_SUCCESS;
		}
	}

	return STATUS_ERROR;

}

int remove_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *removestring) {
	bool found = false;
	for (int i = 0; i < dbhdr->count; i++) {
		int res = strncmp(employees[i].name, removestring, sizeof(employees[i].name));
		if (res == 0) {
			found = true;
		}
		if (found && i+1 < dbhdr->count) {
			employees[i] = employees[i+1];
		}
	}
	if (found) {
		return STATUS_SUCCESS;
	}
	return -1;
}

void clear_file(int fd) {
	if (ftruncate(fd, 0) == STATUS_ERROR) {
		perror("ftruncate");
	}
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
	if (fd < 0) {
		printf("Got a bad FD from the user\n");
		return STATUS_ERROR;
	}

	int count = dbhdr->count;
	
	struct employee_t *employees = calloc(count, sizeof(struct employee_t));
	if (employees == NULL) {
		printf("Calloc failed\n");
		return STATUS_ERROR;
	}

	read(fd, employees, count*sizeof(struct employee_t));
	
	for (int i = 0; i < count; i++) {
		employees[i].hours = ntohl(employees[i].hours);
	}
	*employeesOut = employees;
	return STATUS_SUCCESS;
}

void output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
	if (fd < 0) {
		printf("Got a bad FD from the user\n");
		return;
	}

	unsigned short realcount = dbhdr->count;	
	unsigned int filesize = sizeof(struct dbheader_t) + sizeof(struct employee_t) * realcount;
	
	dbhdr->magic = htonl(dbhdr->magic);
	dbhdr->filesize = htonl(filesize);
	dbhdr->count = htons(dbhdr->count);
	dbhdr->version = htons(dbhdr->version);
	
	lseek(fd, 0, SEEK_SET);
	if (write(fd, dbhdr, sizeof(struct dbheader_t)) == STATUS_ERROR) {
		printf("Failed write dbheader fd = %d\n", fd);
		perror("write");
		return;
	}
	
	printf("realcount = %d\n", realcount);

	for (int i = 0; i < realcount; i++) {
		employees[i].hours =  htonl(employees[i].hours);
		
		if (write(fd, &employees[i], sizeof(struct employee_t)) == STATUS_ERROR) {
			printf("Failed write employee %s, fd = %d", employees[i].name, fd);
			perror("write");
			return;
		}
	}
	
	return;
}

int validate_db_header(int fd, struct dbheader_t **headerOut) {
	if (fd < 0) {
		printf("Got a bad FD from the user\n");
		return STATUS_ERROR;
	}

	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == NULL) {
		printf("Malloc failed create a db header\n");
		return STATUS_ERROR;
	}
	
	if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
		perror("read");
		free(header);	
		return STATUS_ERROR;
	}

	header->version = ntohs(header->version);
	header->count = ntohs(header->count);
	header->magic = ntohl(header->magic);
	header->filesize = ntohl(header->filesize);

	if (header->version != 1) {
		printf("Improper header version\n");
		free(header);
		return -1;
	}
	
	struct stat dbstat = {0};
	fstat(fd, &dbstat);
	if (header->filesize != dbstat.st_size) {
		printf("Corrupted database\n");
		return -1;
	}

	*headerOut = header;
	return 0;
}

int create_db_header(int fd, struct dbheader_t **headerOut) {
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == NULL) {
		printf("Malloc failed ot create db header\n");
		return STATUS_ERROR;
	}	

	header->version = 0x1;
	header->count = 0;
	header->magic = HEADER_MAGIC;
	header->filesize = sizeof(struct dbheader_t);
	
	*headerOut = header;

	return STATUS_SUCCESS;
}
