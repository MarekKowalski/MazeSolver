//	 MazeSolver - a very simple program for visual maze solving.
//   Copyright (C) 2015  Marek Kowalski (M.Kowalski@ire.pw.edu.pl)
//   License: MIT Software License   See LICENSE.txt for the full license.

#include <stdio.h>
#include <math.h>
#include <queue>

#include "jpgd.h"
#include "jpge.h"

void binarizeImage(unsigned char *image, unsigned char *binaryImage, int nPixels, unsigned char threshold)
{
	for (int i = 0; i < nPixels; i++)
	{
		if (image[i] > threshold)
			binaryImage[i] = 255;
		else
			binaryImage[i] = 0;
	}
}

//This function "fixes" some places in the maze where the wall is too thin.
void dilateImage(unsigned char *image, int w, int h)
{
	unsigned char curPix, nextPix, belowPix, acrossPix;
	for (int x = 0; x < w - 1; x++)
	{
		for (int y = 0; y < h - 1; y++)
		{
			curPix = image[x + y * w];
			nextPix = image[x + 1 + y * w];
			belowPix = image[x + (y + 1) * w];
			acrossPix = image[x + 1 + (y + 1) * w];

			if ((curPix == 0 && nextPix == 255 && belowPix == 255 && acrossPix == 0) ||
				(curPix == 255 && nextPix == 0 && belowPix == 0 && acrossPix == 255))
			{
				image[x + y * w] = 0;
				image[x + 1 + y * w] = 0;
				image[x + (y + 1) * w] = 0;
				image[x + 1 + (y + 1) * w] = 0;
			}
		}
	}
}

void getPathThroughMaze(unsigned char *outputImage, unsigned char *binaryImage, int w, int h, int startX, int startY, int endX, int endY, bool drawCostMap)
{
	unsigned char *visited = (unsigned char*)calloc(w * h, 1);
	unsigned int *pathValue = (unsigned int*)calloc(w * h, sizeof(unsigned int));

	std::queue<std::pair<int, int>> currentPoints;
	currentPoints.push(std::pair<int, int>(startX, startY));
	pathValue[startX + startY * w] = 1;
	visited[startX + startY * w] = 1;

	printf("Running Dijkstra's algorithm\n");

	bool endReached = false;
	unsigned int maxPathValue = 0;
	while (currentPoints.size() > 0)
	{
		std::pair<int, int> currPair = currentPoints.front();
		currentPoints.pop();

		int currX = currPair.first;
		int currY = currPair.second;

		unsigned int currPathValue = pathValue[currX + currY * w];
		if (currPathValue > maxPathValue)
			maxPathValue = currPathValue;

		if (currX == endX && currY == endY)
		{
			endReached = true;
			break;
		}

		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (currX + j < 0 || currY + i < 0 || currX + j >= w || currY + i >= h)
					continue;
				if (binaryImage[(currX + j) + (currY + i) * w] && !visited[(currX + j) + (currY + i) * w])
				{
					pathValue[(currX + j) + (currY + i) * w] = currPathValue + 1;
					visited[(currX + j) + (currY + i) * w] = 1;
					currentPoints.push(std::pair<int, int>(currX + j, currY + i));
				}
			}
		}
	}

	if (drawCostMap)
	{
		double multiplier = 1.0 / maxPathValue;
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				for (int i = 0; i < 3; i++)
					outputImage[3 * (x + y * w) + i] = static_cast<unsigned char>(255 * multiplier * pathValue[x + y * w]);
			}
		}

		return;
	}

	if (!endReached)
	{
		printf("Solution to the maze not found\n");
		return;
	}

	//Let's color the maze walls in a black, with gray background so that the path is more visible.
	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			for (int i = 0; i < 3; i++)
			{
				if (binaryImage[x + y * w] == 255)
					outputImage[3 * (x + y * w) + i] = 128;
			}
		}
	}

	printf("Drawing path\n");
	unsigned int currPathValue = pathValue[endX + endY * w];
	int currX = endX;
	int currY = endY;
	while (currPathValue != 1)
	{
		outputImage[3 * (currX + currY * w) + 0] = 0;
		outputImage[3 * (currX + currY * w) + 1] = 255;
		outputImage[3 * (currX + currY * w) + 2] = 0;

		int bestMoveX = 0;
		int bestMoveY = 0;
		unsigned int bestMovePathVal = maxPathValue;
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (i == 0 && j == 0)
					continue;
				if (currX + j < 0 || currY + i < 0 || currX + j >= w || currY + i >= h)
					continue;
				if (pathValue[(currX + j) + (currY + i) * w] == 0)
					continue;
				if (pathValue[(currX + j) + (currY + i) * w] < bestMovePathVal)
				{
					bestMovePathVal = pathValue[(currX + j) + (currY + i) * w];
					bestMoveX = currX + j;
					bestMoveY = currY + i;
				}
			}
		}

		currX = bestMoveX;
		currY = bestMoveY;

		currPathValue = pathValue[currX + currY * w];
	}
}

int main(int argc, char *argv[])
{
	unsigned char *inputImage, *outputImage, *binaryImage;
	int w, h;
	int startX, startY, endX, endY;
	bool saveCostMap = false;

	if (argc < 6)
	{
		printf("Not enough input argmuents. The proper way to run the program is shown below.\n");
		printf("If the program completes successully the path through the maze will be saved in output.jpg\n");
		printf("\n");
		printf("MazeSolver IMAGE_FILE_NAME.jpg START_X START_Y END_X END_Y [SAVE_COST_MAP]\n");
		printf("\n");
		printf("Where:\n");
		printf("\tSTART_X/Y are the pixel coordinates of the maze entrance.\n");
		printf("\tEND_X/Y are the pixel coordinates of the maze exit.\n");
		printf("\t[SAVE_COST_MAP] is an additional paramter, if added the program saves the maze cost map instead of the path through the maze\n");
		return -1;
	}

	if (argc == 7)
		saveCostMap = true;

	startX = atoi(argv[2]);
	startY = atoi(argv[3]);
	endX = atoi(argv[4]);
	endY = atoi(argv[5]);

	int actual_comps;
	inputImage = jpgd::decompress_jpeg_image_from_file(argv[1], &w, &h, &actual_comps, 1);
	outputImage = new unsigned char[w * h * 3]();
	binaryImage = new unsigned char[w * h]();

	binarizeImage(inputImage, binaryImage, w * h, 200);
	dilateImage(binaryImage, w, h);
	getPathThroughMaze(outputImage, binaryImage, w, h, startX, startY, endX, endY, saveCostMap);

	jpge::params params;
	params.m_quality = 100;
	jpge::compress_image_to_jpeg_file("output.jpg", w, h, 3, outputImage, params);

	delete []inputImage;
	delete []outputImage;

	return 0;
}