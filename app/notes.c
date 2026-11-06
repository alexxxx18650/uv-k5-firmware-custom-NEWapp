#include "notes.h"
#include "graphics.h"
#include "keyboard.h"
#include <string.h>

static char note_text[16] = "FREQ: 145.500"; 
static uint8_t cursor_pos = 0;

void UI_DisplayNotes(void)
{
    UI_ClearScreen();
    UI_PrintString("MY NOTES", 10, 118, 1, 8, false);
    UI_DrawLine(0, 12, 127, 12, true);

    UI_PrintString(note_text, 10, 110, 3, 8, false);

    uint8_t cursor_x = 10 + (cursor_pos * 8);
    UI_DrawLine(cursor_x, 34, cursor_x + 6, 34, true);

    UI_DrawLine(0, 52, 127, 52, true);
    UI_PrintString("UP/DN: CHAR | M: NEXT | EXIT: QUIT", 1, 125, 5, 6, false);
    UI_UpdateDisplay();
}

void APP_NotesProcess(void)
{
    KEY_Code_t key;
    cursor_pos = 0;
    
    UI_DisplayNotes();

    while (1) {
        key = KEY_GetPressed();
        
        if (key == KEY_EXIT) {
            break; 
        }

        if (key == KEY_UP) {
            if (note_text[cursor_pos] == '\0' || note_text[cursor_pos] == ' ') {
                note_text[cursor_pos] = 'A';
            } else if (note_text[cursor_pos] == 'Z') {
                note_text[cursor_pos] = '0';
            } else if (note_text[cursor_pos] == '9') {
                note_text[cursor_pos] = '.';
            } else if (note_text[cursor_pos] == '.') {
                note_text[cursor_pos] = ':';
            } else if (note_text[cursor_pos] == ':') {
                note_text[cursor_pos] = ' ';
            } else {
                note_text[cursor_pos]++;
            }
            UI_DisplayNotes();
        }

        if (key == KEY_DOWN) {
            if (note_text[cursor_pos] == ' ' || note_text[cursor_pos] == '\0') {
                note_text[cursor_pos] = ':';
            } else if (note_text[cursor_pos] == ':') {
                note_text[cursor_pos] = '.';
            } else if (note_text[cursor_pos] == '.') {
                note_text[cursor_pos] = '9';
            } else if (note_text[cursor_pos] == '0') {
                note_text[cursor_pos] = 'Z';
            } else if (note_text[cursor_pos] == 'A') {
                note_text[cursor_pos] = ' ';
            } else {
                note_text[cursor_pos]--;
            }
            UI_DisplayNotes();
        }

        if (key == KEY_MENU) {
            cursor_pos++;
            if (cursor_pos >= (sizeof(note_text) - 1)) {
                cursor_pos = 0; 
            }
            UI_DisplayNotes();
        }
    }
}
