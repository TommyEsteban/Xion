/**
* serializer.h
* Guardian Kids
* NestNet Group
*/

#include "../../include/serializer.h"

// this pointer keeps all the raw data of the current image being read
unsigned char *raw_image;

int counter = 0;	// to count the number of files
int width = 0;		// the width of the current image being read
int height = 0;		// the height of the current image being read
int components = 0; // the number of components per pixel
int total = 0;		// the total of components in the image, (width * height * components)
int totalScanline = 0; // the total of components in one iamge scanline (width * components)

void serializeToFile(char *destinationFile, char *additionalData)
{
	// warn the user that the data will be appended if destination file already exist
	//if(counter == 2)
		//printf("If the file %s already exist the data will be appended!\n", destinationFile);
	
	// open the file for appending data
	FILE *output = fopen(destinationFile, "a");
	
	// the file must be available for appending and the raw data of the image must be available
	if(output == NULL)
	{
		printf("destination file could not be open\n");
		return;
	}
	
	if(raw_image == NULL)
	{
		printf("raw_image data is null.\n");
		return;
	}
	
	// iterate for all pixels and write to the file
	for(int i = 0; i < total; i += components)
	{
		fprintf(output, "%d ", raw_image[i]);
	}

	// add the additional data at the end or just the return character
	if(additionalData != NULL)
		fprintf(output, "%s\n", additionalData);
	else
		fprintf(output, "\n");
	
	// close the file
	fclose(output);
}

void appendToFile(char *destinationFile, float *data, unsigned int n)
{
	// warn the user that the data will be appended if destination file already exist
	//if(counter == 2)
		//printf("If the file %s already exist the data will be appended!\n", destinationFile);
	
	// open the file for appending data
	FILE *output = fopen(destinationFile, "a");
	
	// the file must be available for appending and the raw data of the image must be available
	if(output == NULL)
	{
		printf("destination file could not be open\n");
		return;
	}
	
	if(data == NULL)
	{
		printf("data provided is null.\n");
		return;
	}
	
	// iterate for all points of data and write to the file
	for(unsigned int i = 0; i < n; i ++)
	{
		fprintf(output, "%.6f ", data[i]);
	}
	
	// close the file
	fclose(output);
}

void appendToFileInt(char *destinationFile, int *data, unsigned int n)
{
	// warn the user that the data will be appended if destination file already exist
	//if(counter == 2)
		//printf("If the file %s already exist the data will be appended!\n", destinationFile);
	
	// open the file for appending data
	FILE *output = fopen(destinationFile, "a");
	
	// the file must be available for appending and the raw data of the image must be available
	if(output == NULL)
	{
		printf("destination file could not be open\n");
		return;
	}
	
	if(data == NULL)
	{
		printf("data provided is null.\n");
		return;
	}
	
	// iterate for all points of data and write to the file
	for(unsigned int i = 0; i < n; i ++)
	{
		fprintf(output, "%d ", data[i]);
	}
	
	// close the file
	fclose(output);
}

/*void readJpegFile(char *filename)
{
	// these are standard libjpeg structures for reading(decompression) 
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	// libjpeg data structure for storing one row, that is, scanline of an image 
	JSAMPROW row_pointer[1];
	
	FILE *infile = fopen(filename, "rb");
	unsigned long location = 0;
	int i = 0;
	
	if ( !infile )
	{
		printf("Error opening jpeg file %s!\n", filename);
		return;
	}
	
	// here we set up the standard libjpeg error handler 
	cinfo.err = jpeg_std_error(&jerr);
	
	// setup decompression process and source, then read JPEG header 
	jpeg_create_decompress(&cinfo);
	
	// this makes the library read from infile 
	jpeg_stdio_src(&cinfo, infile);
	
	// reading the image header which contains image information 
	jpeg_read_header(&cinfo, TRUE);
	
	// Start decompression jpeg here 
	jpeg_start_decompress(&cinfo);

	// allocate memory to hold the uncompressed image 
	width = cinfo.output_width;
	height = cinfo.output_height;
	components = cinfo.num_components;
	total = cinfo.output_width * cinfo.output_height * cinfo.num_components;
	totalScanline = cinfo.output_width * cinfo.num_components;
	//printf("width: %d, height: %d, components: %d\n", width, height, components);
	raw_image = (unsigned char*)malloc(total);
	
	if(raw_image == NULL)
		return;
	
	// now actually read the jpeg into the raw buffer 
	row_pointer[0] = (unsigned char *)malloc(cinfo.output_width * cinfo.num_components);
	
	// read one scan line at a time 
	while(cinfo.output_scanline < cinfo.image_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for(i = 0; i < totalScanline; i++) 
			raw_image[location++] = row_pointer[0][i];
	}
	
	// wrap up decompression, destroy objects, free pointers and close open files
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	fclose(infile);
}*/

void readJpegFile(char *filename, unsigned char **data, int *components)
{
	// these are standard libjpeg structures for reading(decompression) 
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	// libjpeg data structure for storing one row, that is, scanline of an image 
	JSAMPROW row_pointer[1];
	
	FILE *infile = fopen(filename, "rb");
	unsigned long location = 0;
	int i = 0;
	
	if ( !infile )
	{
		printf("Error opening jpeg file %s!\n", filename);
		return;
	}
	
	// here we set up the standard libjpeg error handler 
	cinfo.err = jpeg_std_error(&jerr);
	
	// setup decompression process and source, then read JPEG header 
	jpeg_create_decompress(&cinfo);
	
	// this makes the library read from infile 
	jpeg_stdio_src(&cinfo, infile);
	
	// reading the image header which contains image information 
	jpeg_read_header(&cinfo, TRUE);
	
	// Start decompression jpeg here 
	jpeg_start_decompress(&cinfo);

	// allocate memory to hold the uncompressed image 
	width = cinfo.output_width;
	height = cinfo.output_height;
	//components = cinfo.num_components;
	(*components) = cinfo.num_components;
	total = cinfo.output_width * cinfo.output_height * cinfo.num_components;
	totalScanline = cinfo.output_width * cinfo.num_components;
	//printf("width: %d, height: %d, components: %d\n", width, height, components);
	*data = (unsigned char*)malloc(total);
	
	if(*data == NULL)
		return;
	
	// now actually read the jpeg into the raw buffer 
	row_pointer[0] = (unsigned char *)malloc(cinfo.output_width * cinfo.num_components);
	
	// read one scan line at a time 
	while(cinfo.output_scanline < cinfo.image_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for(i = 0; i < totalScanline; i++) 
			(*data)[location++] = row_pointer[0][i];
	}
	
	// wrap up decompression, destroy objects, free pointers and close open files
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	fclose(infile);
}

void writeJpegFile(char *destinationFile, unsigned char *pixels, int w, int h, int b)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];
	FILE *outfile = fopen(destinationFile, "wb");
	
	if (!outfile)
	{
		printf("error opening output jpeg file %s\n!", destinationFile);
		return;
	}
	
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	/* Setting the parameters of the output file here */
	cinfo.image_width = w;	
	cinfo.image_height = h;
	cinfo.input_components = b;
	cinfo.in_color_space = JCS_RGB;
	
    /* default compression parameters, we shouldn't be worried about these */
	jpeg_set_defaults(&cinfo);
	
	jpeg_set_quality(&cinfo, 100, TRUE);
	
	/* Now do the compression .. */
	jpeg_start_compress(&cinfo, TRUE);
	
	/* like reading a file, this time write one row at a time */
	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &pixels[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );
}

/*void appendImageData(char *sourceFolder, char *destinationFile, char *additionalData)
{
	DIR *dir;
	struct dirent *ent;
	counter = 0;
	
	if ((dir = opendir (sourceFolder)) != NULL) 
	{
		// read files in the directory one by one
		while ((ent = readdir (dir)) != NULL) 
		{
			// skip . and .. entries
			if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				int len1 = strlen(sourceFolder);
				int len2 = strlen(ent->d_name);
				
				// variable to concat the directory with the file name
				char *filePath = (char *)malloc(len1 + len2 + 2);
				
				if(filePath == NULL)
					return;
					
				strcpy(filePath, sourceFolder);
				strcat(filePath, "/");
				strcat(filePath, ent->d_name);
				//printf("%d %s\n", counter + 1, filePath);

				readJpegFile(filePath);
				serializeToFile(destinationFile, additionalData);
				//printf ("%d %d %d %d %d %d\n", raw_image[0], raw_image[1], raw_image[2], raw_image[3], raw_image[4], raw_image[5]);
				free(raw_image);
				
				free(filePath);
				counter++;
			}
		}
		
		closedir (dir);
	} 
	else 
	{
		printf("Could not open directory.\n");
		return;
	}
	
	printf("Serialized %d image files.\n", counter);
}*/

void serializeValues(int a, float b, int c, char *destinationFile)
{
	// open the file for appending data
	FILE *output = fopen(destinationFile, "a");
	
	// the file must be available for appending and the raw data of the image must be available
	if(output == NULL)
	{
		printf("destination file could not be open\n");
		return;
	}
	
	fprintf(output, "%d,", a);
	fprintf(output, "%.12f,", b);
	fprintf(output, "%d,", c);
	fprintf(output, "\n");
	
	fclose(output);
}

void serializeArray(float *a, unsigned int size, char *destinationFile)
{
	// open the file for appending data
	FILE *output = fopen(destinationFile, "a");
	
	// the file must be available for appending and the raw data of the image must be available
	if(output == NULL)
	{
		printf("destination file could not be open\n");
		return;
	}

	for(unsigned int i = 0; i < size; i++)
	{
		fprintf(output, "%.12f,", a[i]);
	}
	
	fprintf(output, "\n");
    fclose(output);
}

