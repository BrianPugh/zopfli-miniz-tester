valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./compare LA.gbc LA.zopfli
