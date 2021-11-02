
unsigned char atord(char c)
{
        if (c >= 'A' && c <= 'Z'){
                return (c - 'A') + 1;
        } else if (c >= 'a' && c <= 'z') {
                return (c - 'a') + 1;
        } else {
                return 0;
        }
}
