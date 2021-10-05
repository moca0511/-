
#!/usr/bin/bash
nowDir=$('pwd')
echo ${nowDir}
find ./ -name "*.o"  -type f | xargs rm
find ./ -name "*.a"  -type f | xargs rm
find ./ -name "*.out"  -type f | xargs rm
