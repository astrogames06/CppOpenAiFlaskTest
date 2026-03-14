#ifndef RAYLIBEXTRA_H
#define RAYLIBEXTRA_H

#include <raylib.h>

int DrawTextWrapped(const char *text, int posX, int posY, int fontSize, int maxWidth, Color color)
{
    int start = 0;
    int len = TextLength(text);

    char line[1024] = {0};
    int lineY = posY;
    int lines = 0;

    int lineHeight = fontSize + fontSize/2; // better spacing

    while (start < len)
    {
        int bestBreak = start;
        int i = start;

        while (i < len)
        {
            int lineLen = i - start + 1;
            if (lineLen >= 1023) break;

            TextCopy(line, TextSubtext(text, start, lineLen));

            if (MeasureText(line, fontSize) > maxWidth) break;

            if (text[i] == ' ') bestBreak = i;

            i++;
        }

        int breakPos;

        if (i >= len)
        {
            breakPos = len;
        }
        else
        {
            breakPos = (bestBreak > start) ? bestBreak : i;
        }

        TextCopy(line, TextSubtext(text, start, breakPos - start));
        DrawText(line, posX, lineY, fontSize, color);

        lines++;
        lineY += lineHeight;
        start = breakPos + 1;
    }

    return lines * lineHeight;
}

#endif