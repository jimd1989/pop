## THIS BRANCH IS BROKEN; IGNORE UNTIL MERGED.

# pop

A dead simple note/command playback queue. For use in live performance with any signal-driven instrument.

## Build

    cc pop.c -o pop

## Usage

    pop <score data file>

Once called with a single file as its argument, `pop` will wait for input from stdin. When it receives a signal, it will spit one line of the file to stdout, for further processing. When the file returns an EOF, `pop` will open it again and start playing from the beginning. The loop has to be manually broken with ^c. Both the input and output are arbitrary. You can drive the tempo of the performance by manually pressing enter if you really want to, but I suggest using it in a pipe. 

Here I have a TR-606's sync pulse plugged into my microphone socket, where [bang](https://github.com/jim1989/bang) is listening. When it encounters the pulse it pipes an "a" into `pop`, which then fires off a note into a FIFO where my synthesizer [boar](https://github.com/jimd1989/boar) is waiting for input. `boar`'s parameters can be edited in real time by echoing more commands to the same FIFO. Analog and digital in harmony!

    mkfifo performance
    boar <> performance
    bang 142 12 a | pop score > performance
    echo "L 0.5" > performance
    echo "P 2.0" > performance
    ...

## Caveats

Scores are monophonic at the moment. One pulse: one command. I don't know if that is a problem.
