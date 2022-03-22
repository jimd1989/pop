## THIS BRANCH IS BROKEN; IGNORE UNTIL MERGED.

# pop

A dead simple note/command playback queue. You give it a line of input, it gives you the next line of a file. Written for live music performances involving pulse signals, but it is fully plaintext and unixy, so use it as you please.

## Build

    make && make install

Tested on MacOS and OpenBSD. Don't care about anything else. Hack it accordingly if this doesn't work.

## Usage

    pop <score data files ...>

### Basic

Once called with at least one file as its argument, `pop` will wait for stdin input. When it receives any signal, including a blank line, it will spit one line of the file to stdout, where it can be processed accordingly. When the file returns EOF, `pop` will rewind and read it from the start once more. The loop has to be broken with ^c.

It mostly makes sense to run `pop` with pipes and fifos.

### Advanced

`pop` can switch between multiple files: up to 32—edit code if you need more. When the `s<n>` command is issued, where `<n>` is the file arg number, `pop` will begin reading from arg `<n>` after it has read through all of its current file. A `s` command is special input that will not advance the current file by a line. This "on deck" behavior is geared towards live music performance, much like selecting the next pattern on a drum machine.

It can also switch with a capital `S<n>` command. This will jump immediately to a new file; the next input will yield the first line of file `<n>`. One can rewind a file early by providing an `<n>` that is the same number as the current file. This is a good way to create irregular rhythms.

## Examples

Here I have a TR-606's sync pulse plugged into my microphone socket, where [bang](https://github.com/jim1989/bang) is listening. When it encounters the pulse it pipes an "a" into `pop`, which then fires off a note into a FIFO where my synthesizer [boar](https://github.com/jimd1989/boar) is waiting for input. `boar`'s parameters can be edited in real time by echoing more commands to the same FIFO. Analog and digital in harmony!

    mkfifo performance
    boar <> performance
    bang 142 12 a | pop score > performance
    echo "L 0.5" > performance
    echo "P 2.0" > performance
    ...

## Caveats

Scores are monophonic at the moment. One pulse: one command. I don't know if that is a problem.
