#ifndef TEXTURE_COMPRESSOR_H
#define TEXTURE_COMPRESSOR_H

void Init();
void LoadAndSerializeImageFile(const char* filepath, const char* outputFolder);
void CompressImageFile(const char* filepath, const char* outputFolder, bool gammaSpace = false);
void LoadAndSerializeFontFile(const char* filepath, const char* outputFolder);

#endif