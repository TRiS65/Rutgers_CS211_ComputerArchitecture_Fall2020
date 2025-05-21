 write a cache simulator using the C programming language

 Part 1- One Level Cache：
 
   --Cache Simulator Interface
   
   --Cache Replacement Policy：FIFO & LRU
   
   --Simulation Details
   
         $./ rst 32 assoc:2 fo 4 trace2.txt
         
         memread:3499
         
         memwrite:2861
         
         cachehit:6501
         
         cachemiss:3499

Part 2- Two Level Cache

  -- Exclusive Cache

  -- Sample Run：

         $./second 32 assoc:2 fo 4 64 assoc lru trace2.txt
         
         memread:3277
         
         memwrite:2861
         
         l1cachehit:6501
         
         l1cachemiss:3499
         
         l2cachehit:222
         
         l2cachemiss:3277
   
