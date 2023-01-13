# cve_bin_tool_optimization

## Describe
The [cve-bin-tool](https://github.com/intel/cve-bin-tool) is a free, open source tool to help you find known vulnerabilities in software, using data from the National Vulnerability Database (NVD) list of Common Vulnerabilities and Exposures (CVEs).   

When scanning files with large volume, the scanning speed of cve-bin-tool is very slow.


The reason is that the aio_parse function in strings.py, reduce scanning time by using C to implement aio_parse function.

## Instructions
1.Backup string.py

2.Place string.py and string.dll in the cve-bin-tool directory
