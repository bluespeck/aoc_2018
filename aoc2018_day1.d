import std.algorithm.iteration;
import std.container.rbtree;
import std.conv;
import std.stdio;
import std.string;


alias FreqChangeVector = int[];

int GetResultingFreq(in ref FreqChangeVector freqChanges)
{
    return sum(freqChanges);
}

int GetFirstFreqReachedTwice(in ref FreqChangeVector freqChanges)
{
    auto frequencies = redBlackTree!int;

    int freq = 0;
    while (true)
    {
        foreach (int freqChange ; freqChanges)
        {
            freq += freqChange;
            if (!frequencies.equalRange(freq).empty)
            {
                return freq;
            }
            else
            {
                frequencies.insert(freq);
            }
        }
    }
}

FreqChangeVector ReadInput()
{
    FreqChangeVector freqChanges;

    int freqChange;
    while(!stdin.eof)
    {
		string line = readln();
		freqChange = parse!int(line);
		freqChanges.length ++;
        freqChanges[$ - 1] = freqChange;
    }

    return freqChanges;
}

int main()
{
	FreqChangeVector freqChanges = ReadInput();

    writefln("%d", GetResultingFreq(freqChanges));
	writefln("%d", GetFirstFreqReachedTwice(freqChanges));
    return 0;
}
