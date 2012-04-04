#include "Document.h"

string& cleanWord(string& word)
{
    for(unsigned i = 0; i < word.length(); i++)
    {
        if (isalpha(word[i]))
            word[i] = tolower(word[i]);
        else if (!isdigit(word[i]))
            word.erase(i--,1);
    }

    return word;
}
