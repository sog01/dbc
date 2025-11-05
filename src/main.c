#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[])
{
	printf("Usage: %s -n -f <database file\n", argv[0]);
	printf("\t -n - create new database file\n");
	printf("\t -f - (required path to database file\n");
	return;
}

int main(int argc, char *argv[])
{
	// char *filepath = NULL;
	// bool newfile = false;
	// char *addstring = NULL;
	// bool list = false;
	// char *remove = NULL;
	// char *updatestring = NULL;
	// int c;

	// int dbfd = -1;
	// struct dbheader_t *dbhdr = NULL;
	// struct employee_t *employees = NULL;

	// while ((c = getopt(argc, argv, "nf:a:lr:u:")) != -1)
	// {
	// 	switch (c)
	// 	{
	// 	case 'n':
	// 		newfile = true;
	// 		break;
	// 	case 'f':
	// 		filepath = optarg;
	// 		break;
	// 	case 'a':
	// 		addstring = optarg;
	// 		break;
	// 	case 'l':
	// 		list = true;
	// 		break;
	// 	case 'r':
	// 		remove = optarg;
	// 		break;
	// 	case 'u':
	// 		updatestring = optarg;
	// 		break;
	// 	case '?':
	// 		printf("Unknown option -%c\n", c);
	// 		break;
	// 	default:
	// 		return -1;
	// 	}
	// }

	// if (!filepath)
	// {
	// 	printf("Filepath is a required argument\n");
	// 	print_usage(argv);
	// 	return 1;
	// }

	// if (newfile)
	// {
	// 	dbfd = create_db_file(filepath);
	// 	if (dbfd == STATUS_ERROR)
	// 	{
	// 		printf("Unable to create database file\n");
	// 		return STATUS_ERROR;
	// 	}
	// 	if (create_db_header(&dbhdr) == STATUS_ERROR)
	// 	{
	// 		printf("Failed to create datbase header\n");
	// 		return STATUS_ERROR;
	// 	}
	// }
	// else
	// {
	// 	dbfd = open_db_file(filepath);
	// 	if (dbfd == STATUS_ERROR)
	// 	{
	// 		printf("Unable to open database file\n");
	// 		return STATUS_ERROR;
	// 	}
	// 	if (validate_db_header(dbfd, &dbhdr) == STATUS_ERROR)
	// 	{
	// 		printf("Failed to validate database header\n");
	// 		return STATUS_ERROR;
	// 	}
	// }

	// if (read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS)
	// {
	// 	printf("Failed to read employees");
	// 	return 0;
	// }

	// if (addstring)
	// {
	// 	add_employee(dbhdr, &employees, addstring);
	// }

	// if (remove)
	// {
	// 	if (remove_employee(dbhdr, employees, remove) == STATUS_SUCCESS)
	// 	{
	// 		dbhdr->count--;
	// 		employees = realloc(employees, dbhdr->count * (sizeof(struct employee_t)));
	// 		clear_file(dbfd);
	// 	}
	// 	else
	// 	{
	// 		printf("Employee %s not found\n", remove);
	// 	}
	// }

	// if (updatestring)
	// {
	// 	if (update_employee(dbhdr, employees, updatestring) == STATUS_ERROR)
	// 	{
	// 		printf("Employee %s not found\n", updatestring);
	// 	}
	// }

	// if (list)
	// {
	// 	list_employees(dbhdr, employees);
	// }

	// output_file(dbfd, dbhdr, employees);

	return 0;
}
