#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "checksum.h"

int main(int argc, char** argv)
{
	FILE*    f;

	const int header_size = (int)sizeof(vmHeader_t);
	vmHeader_t* header = NULL;
	
	int sz, fsz;

	uint8_t* payload = NULL;
	int payload_size;

	uint8_t* image = NULL;

	const char ext_header_marker_ro[]="q3vmext\0";
	char ext_header_marker[8];

	if (argc < 2)
    {
        printf("No virtual machine file supplied. Example: q3vmx bytecode.qvm\n");
        return -1;
    }

    char* filepath = argv[1];

    /* open file as bynary read and append */
    f = fopen(filepath, "a+b");
    if (!f)
    {
        fprintf(stderr, "Failed to open file %s.\n", filepath);
        return -1;
    }

    header = (vmHeader_t*)malloc(header_size);

    if (!header) {
    	fclose(f);
    	return -1;
    }

    /* read Q3VM header from file */

    if (fread((uint8_t*)header, 1, header_size, f) != (size_t)header_size)
    {
        free(header);
        fclose(f);
        fprintf(stderr, "File is corrupt.\n");
        return -1;
    }

    /* check the Q3VM header */
    header->vmMagic = LittleLong(header->vmMagic);

    if (header->vmMagic == VM_MAGIC)
    {
    	printf("Magic ok.\n");
    	header->instructionCount = LittleLong(header->instructionCount);
    	header->codeOffset = LittleLong(header->codeOffset);
    	header->codeLength = LittleLong(header->codeLength);
    	header->dataOffset = LittleLong(header->dataOffset);
    	header->dataLength = LittleLong(header->dataLength);
    	header->litLength = LittleLong(header->litLength);
    	header->bssLength = LittleLong(header->bssLength);
    }
    else 
    {
    	free(header);
        fclose(f);
    	fprintf(stderr, "Wrong header magic number.\n");
    	return -1;
    }

    /* validate */
    if (header->bssLength < 0 || header->dataLength < 0 ||
        header->litLength < 0 || header->codeLength <= 0 ||
        header->codeOffset < 0 || header->dataOffset < 0 ||
        header->instructionCount <= 0 ||
        header->bssLength > VM_MAX_BSS_LENGTH)
    {
        free(header);
        fclose(f);
        printf("Error: %s has bad header\n");
        return -1;
    }

    rewind(f);
    fseek(f, 0L, SEEK_END);
    sz = ftell(f);

    printf("File size %d.\n", sz);

    /* криво! а вдруг пусты промежутки между областями,
    *  offset надо использовать тоже 
    */
    int calculated_size = header_size + header->codeLength 
    	+ header->dataLength + header->litLength;
    printf("Calculated file size %d.\n", calculated_size);

    if (sz < calculated_size)
    {
    	free(header);
        fclose(f);
        fprintf(stderr, "File is corrupt.\n");
        return -1;
    }

    /* add q3vmx header */

    if (sz > calculated_size)
    {
    	/* сделать проверку */
    	fseek(f, calculated_size, SEEK_SET);

    	if (fread(ext_header_marker, 1, sizeof(ext_header_marker), f) != sizeof(ext_header_marker))
    	{
    		printf("something wrong.\n");
    		free(header);
        	fclose(f);
        	return -1;
    	}
    	else
    	{
    		printf("Found header %s\n", ext_header_marker);

    		if (strncmp(ext_header_marker, ext_header_marker_ro, strlen(ext_header_marker_ro)) != 0)
    		{
    			printf("Wrong header.\n");
    			free(header);
		        fclose(f);
		        return -1;
    		}
    		
    		
    	}
    }
    else
    {
    	printf("No q3vmx header.\n");
    	free(header);
        fclose(f);
        return -1;
    }

    fseek(f, 0, SEEK_SET);

    image = (uint8_t*)malloc(calculated_size);

    if (!image) {
    	fclose(f);
    	free(header);
    	return -1;
    }

    if (fread((uint8_t*)image, 1, calculated_size, f) != (size_t)calculated_size)
    {
        free(image);
        free(header);
        fclose(f);
        fprintf(stderr, "File is corrupt.\n");
        return -1;
    }

    /* calculate crc16 */

    uint16_t image_crc16 = crc_16(image, calculated_size);

    printf("CRC16 %d.\n", image_crc16);

    printf("We are here.\n");
    
    free(header);
    fclose(f);
    return 0;
}

#if 1 
/* Callback from the VM that something went wrong */
void Com_Error(vmErrorCode_t level, const char* error)
{
    fprintf(stderr, "Err (%i): %s\n", level, error);
    exit(level);
}

/* Callback from the VM for memory allocation */
void* Com_malloc(size_t size, vm_t* vm, vmMallocType_t type)
{
    (void)vm;
    (void)type;
    printf("Malloc size: %d\n", size);
    return malloc(size);
}

/* Callback from the VM for memory release */
void Com_free(void* p, vm_t* vm, vmMallocType_t type)
{
    (void)vm;
    (void)type;
    free(p);
}
#endif
