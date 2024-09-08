# Candlesticks

Working on simulating a price feed to make candlestick charts out of using raylib. WIP/Under Construction

For compiling the price feed:
```
gcc -o feed feed.c -lm
```

For the chartmaker:  
```
gcc -o chartmaker chartmaker.c -lraylib -lm -lpthread -ldl
```

Receiver is just standard compile right now with no additional flags.
