fn main() {
    let n = 100000000;
    let start = std::time::Instant::now();
    let sum: u64 = (1..n).sum();
    let stop = std::time::Instant::now();
    println!(
        "Rust: Result = {}, Execution time = {:?}",
        sum,
        stop.duration_since(start)
    );
}
