
CHAR szFileName[_MAX_PATH];
CHAR szFileTitle[_MAX_PATH];

//////////////////////////////////////////////////////////////////////////
// Load game from disk.
//////////////////////////////////////////////////////////////////////////
BOOL LoadGame(HANDLE hwnd)
{
    OPENFILENAME ofn;
    char LocCurPath[_MAX_PATH];
    sprintf(LocCurPath, "%s\\%s", CurPath, "Save");
    szFileName[0] = '\0';

    memset(&ofn, 0, sizeof(ofn));

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = GetActiveWindow();
    switch (gamecode)
    {
        case 1: ofn.lpstrFilter = "Socoban(*.soc)\0*.soc\0";
            break;
        case 2: ofn.lpstrFilter = "Spot(*.spo)\0*.spo\0";
            break;
        case 3: ofn.lpstrFilter = "Rotms(*.rot)\0*.rot\0";
            break;
    }
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 0;
    ofn.lpstrFile         = szFileName;
    ofn.nMaxFile          = _MAX_PATH;
    ofn.lpstrInitialDir   = LocCurPath;
    ofn.lpstrFileTitle    = szFileTitle;
    ofn.nMaxFileTitle     = _MAX_PATH;
    ofn.lpstrTitle        = "Load file";
    ofn.lpstrDefExt       = NULL;
    ofn.Flags             = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName((LPOPENFILENAME)&ofn) == FALSE) return FALSE;

    switch (gamecode)
    {
        case 1: p1->LoadGame(szFileName); // Load  game Socoban.
            break;
        case 2: p2->LoadGame(szFileName); // Load game Spot.
            break;
        case 3: p3->LoadGame(szFileName); // Load game Rotms.
            break;
    }

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Save game to disk.
//////////////////////////////////////////////////////////////////////////
BOOL SaveGame(HANDLE hwnd)
{
    OPENFILENAME ofn;
    char LocCurPath[_MAX_PATH];
    sprintf(LocCurPath, "%s\\%s", CurPath, "Save");

    memset(&ofn, 0, sizeof(ofn));

    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
    switch (gamecode)
    {
        case 1: ofn.lpstrFilter = "Socoban(*.soc)\0*.soc\0";
            break;
        case 2: ofn.lpstrFilter = "Spot(*.spo)\0*.spo\0";
            break;
        case 3: ofn.lpstrFilter = "Rotms(*.rot)\0*.rot\0";
            break;
    }
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 0;
    ofn.lpstrFile         = szFileName;
    ofn.nMaxFile          = _MAX_PATH;
    ofn.lpstrInitialDir   = LocCurPath;
    ofn.lpstrFileTitle    = szFileTitle;
    ofn.nMaxFileTitle     = _MAX_PATH;
    ofn.Flags             = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

    if (GetSaveFileName((LPOPENFILENAME)&ofn) == FALSE) return FALSE;

    switch (gamecode)
    {
        case 1: if (strcmp(&(szFileName[strlen(szFileName)-4]), ".soc"))
                strcat(szFileName, ".soc");
            p1->SaveGame(szFileName); // Save game Socoban.
            break;
        case 2: if (strcmp(&(szFileName[strlen(szFileName)-4]), ".spo"))
                strcat(szFileName, ".spo");
            p2->SaveGame(szFileName); // Save game Spot.
            break;
        case 3: if (strcmp(&(szFileName[strlen(szFileName)-4]), ".rot"))
                strcat(szFileName, ".rot");
            p3->SaveGame(szFileName); // Save game Rotms.
            break;
    }

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////

