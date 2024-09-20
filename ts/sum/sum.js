var n = 100000000;
var sum = 0;
var start = Date.now();
for (var i = 0; i <= n; i++) {
    sum += i;
}
var end = Date.now();
console.log("Execution time: ".concat(end - start, " ms"));
console.log(sum);
var formatMemoryUsage = function (data) { return "".concat(Math.round(data / 1024 / 1024 * 100) / 100, " MB"); };
var memoryData = process.memoryUsage();
var memoryUsage = {
    rss: "".concat(formatMemoryUsage(memoryData.rss), " -> Resident Set Size - total memory allocated for the process execution"),
    heapTotal: "".concat(formatMemoryUsage(memoryData.heapTotal), " -> total size of the allocated heap"),
    heapUsed: "".concat(formatMemoryUsage(memoryData.heapUsed), " -> actual memory used during the execution"),
    external: "".concat(formatMemoryUsage(memoryData.external), " -> V8 external memory"),
};
console.log(memoryUsage);
