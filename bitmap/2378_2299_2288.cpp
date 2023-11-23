#include<iostream>
#include<string>
#include<fstream>
#include<cstdint>
#include<cstring>

using namespace std;
#pragma pack(push, 1)
struct BitmapHeader {
	uint16_t fileType;
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
};

struct BitmapInfoHeader {
	uint32_t headerSize;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bitsPerPixel;
	uint32_t compression;
	uint32_t imageSize;
	int32_t xPixelsPerMeter;
	int32_t yPixelsPerMeter;
	uint32_t colorsUsed;
	uint32_t colorsImportant;
};
#pragma pack(pop)
void readfile(string fileName, BitmapHeader& bh, BitmapInfoHeader& bih) {
	ifstream inputFile(fileName, ios::binary);

	if (!inputFile.is_open()) {
		cerr << "Can't open the file!!!" << endl;
		return;
	}

	inputFile.read(reinterpret_cast<char*>(&bh), sizeof(bh));
	inputFile.read(reinterpret_cast<char*>(&bih), sizeof(bih));
	cout << "Bit Depth: " << bih.bitsPerPixel << "-bit" << endl;
	cout << "Image Width: " << bih.width << " pixels" << endl;
	cout << "Image Height: " << bih.height << " pixels" << endl;
	inputFile.close();
}
void writePalette(string fileName, BitmapHeader bh, BitmapInfoHeader bih) {
	if (bih.bitsPerPixel <= 8) {
		ofstream outFile("palette.txt");
		uint32_t numColors = bih.colorsUsed == 0 ? (1 << bih.bitsPerPixel) : bih.colorsUsed;
		outFile << numColors << endl;


		for (uint32_t i = 0; i < numColors; ++i) {
			uint8_t color[4];
			ifstream inputFile(fileName, ios::binary);
			inputFile.read(reinterpret_cast<char*>(color), sizeof(color));
			inputFile.close();
			outFile << static_cast<int>(color[2]) << " " << static_cast<int>(color[1]) << " " << static_cast<int>(color[0]) << endl;
		}
		cout << "Color palette: YES" << endl;
		outFile.close();
	}
	else {
		cerr << "Color palette isn't exit....." << endl;
		return;
	}
}
void writePixel(string fileName, BitmapHeader bh, BitmapInfoHeader bih) {
	ofstream pixelFile("pixel.txt");
	ifstream inputFile(fileName, ios::binary);
	pixelFile << bih.height << " " << bih.width << endl;

	uint32_t rowSize = ((bih.bitsPerPixel * bih.width + 31) / 32) * 4;

	for (int y = bih.height - 1; y >= 0; --y) {
		inputFile.seekg(bh.dataOffset + y * rowSize, ios::beg);
		for (int x = 0; x < bih.width; ++x) {
			uint8_t pixel[4];
			inputFile.read(reinterpret_cast<char*>(pixel), sizeof(pixel));

			pixelFile << static_cast<int>(pixel[2]) << " " << static_cast<int>(pixel[1]) << " " << static_cast<int>(pixel[0]) << " ";
		}

		pixelFile << endl;
	}

	inputFile.close();
	pixelFile.close();
	return;
}
int main(int argc, char** argv) {
	if (argc == 2) {
		BitmapHeader bh;
		BitmapInfoHeader bih;
		cout << argv[1] << endl;
		readfile(argv[1], bh, bih);
		writePalette(argv[1], bh, bih);
		writePixel(argv[1], bh, bih);
	}
}