//========= Copyright Reep Softworks, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================//
#include "UltraEngine.h"
#include "Utilities/ParseArguments.h"
using namespace UltraEngine;

void SystemPause()
{
#if !defined (__APPLE__)
    Print("Press ENTER to continue...");
#else
    Print("Press RETURN to continue...");
#endif
    std::cin.ignore();
}

static const int DisplayHelp()
{
	Print("Application built: " + std::string(__DATE__) + " at: " + std::string(__TIME__) + "\n");
	Print("Usage: \n===========");
	Print("This program will convert texture files to tga files.");
#if defined (_WIN32)
    Print("\n\tExample: path/to/tools/tex2tga.exe [file1] [file2] ...\n");
#else
    Print("\n\tExample: path/to/tools/tex2tga [file1] [file2] ...\n");
#endif
    Print("Arguments: \n===========");
    Print("\tNONE\n");
    Print("Flags: \n===========");
    Print("\t-png : When set, this will output the texture as png file instead of a tga.\n");

    SystemPause();
	return 0;
}

static const bool DecompileTexture(const WString& pszInput, const WString& pszOutput)
{
    auto file = pszInput;

    auto input_ext = ExtractExt(file);
    if (input_ext == "mat" || input_ext == "json" || input_ext == "meta")
        return false;

    // Load the image
    std::shared_ptr<Pixmap> pixmap = LoadPixmap(pszInput);
    if (pixmap == NULL)
    {
        Print("Error: Failed to load pixmap  \"" + pszInput + "\"...");
        return false;
    }

    //Convert to RGBA if needed
    if (pixmap->format != TEXTURE_RGBA)
    {
        pixmap = pixmap->Convert(TEXTURE_RGBA);
    }

    // Resave the image
    if (!pixmap->Save(pszOutput))
    {
        Print(L"Error: Failed to save image  \"" + pszOutput + L"\"...");
        return false;
    }

    return true;
}

void LoadFiles(const WString& path, std::vector<WString>& files)
{
    auto dir = LoadDir(path);
    for (const auto& file : dir)
    {
        auto filepath = path;
        if (not filepath.empty()) filepath += L"/";
        filepath += file;
        switch (FileType(filepath))
        {
        case 1:
            files.push_back(filepath);
            break;
        case 2:
            LoadFiles(filepath, files);
            break;
        }
    }
}

int main(int argc, const char* argv[])
{
    Print("Reep Softworks - tex2tga (" + std::string(__DATE__) + ")");

    // Parse files from the command-line.
    ParseArguments(argc, argv);
    auto droppedfiles = GetDroppedFiles();
    auto droppeddirs = GetDroppedDirectories();

    const bool vb = CheckArgument("v");
    if (vb) Print("Application Started, files discovered: " + std::to_string(droppedfiles.size()));
    if (droppedfiles.empty())
    {
        if (vb) Print("Directories discovered: " + std::to_string(droppeddirs.size()));
        if (!droppeddirs.empty())
        {
            for (const auto& p : droppeddirs)
            {
                LoadFiles(p, droppedfiles);
            }
        }
        else
        {
            return DisplayHelp();
        }
    }

    // If we're still empty, abort.
    if (droppedfiles.empty()) return DisplayHelp();
        
    // Find the Plugin directory.
    // If we can't find a plugins folder in the app directory, asssume it's under the "Tools" folder.
    WString plugin_path = "Plugins";
    if (FileType(plugin_path) != 2)
    {
        ChangeDir("../");
        if (FileType(plugin_path) != 2)
        {
            Print("Error: Failed to locate plugin folder.");
            return DisplayHelp();
        }
    }

    // Load the image plugin.
    auto image_plugin = UltraEngine::LoadPlugin(plugin_path + "/FITextureLoader");
    if (!image_plugin)
    {
        Print("Error: Failed to load FITextureLoader plugin!");
        return 1;
    }

    // Decompile the textures to a tga file by default.
    WString ext = "tga";

    // if "-png" is set, decompile it as a png instead.
    if (CheckArgument("png")) ext = "png";

    shared_ptr<Plugin> basis_plugin = NULL;
    int32 count = 0;
    for (const auto& file : droppedfiles)
    {
        auto in = file;

        // Load basis plugin if we're trying to decompile a ktx texture.
        if (ExtractExt(in) == "basis")
        {
            if (basis_plugin==NULL) basis_plugin = UltraEngine::LoadPlugin(plugin_path + "/Basis");
            Assert(basis_plugin, "Error: Failed to load Basis plugin!");
        }

        auto out = UltraEngine::StripExt(file) + "." + ext;
        if (DecompileTexture(in, out)) count++; 
    }

    image_plugin = NULL;
    basis_plugin = NULL;

    Print("Total files converted: " + std::to_string(count) + "/" + std::to_string(droppedfiles.size()));

#ifdef DEBUG
    SystemPause();
#endif

    return 0;
}