# Sample: layouts

This sample shows how to define and manage layouts to get different representations (e.g. full score, condensed score, individual parts) of the same score. 

When running the sample, the test score 'layouts-sample.xml' must be placed in the same folder than the binaries. Use the 'Layouts' item from main menu to choose an alternative layout for the tests score.


## Building in Linux for wxWidgets

```
g++ -std=c++11 layouts-wx.cpp -o layouts-wx `pkg-config --cflags liblomse` `wx-config --cflags` `pkg-config --libs liblomse` `wx-config --libs` -lstdc++
```

