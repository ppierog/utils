public class Sum {

	public static void main(String[] args){
    int n = 100000000;
    long sum = 0; 
    long startTime = System.currentTimeMillis();
    for (int i =0;i<=n;i++)
      sum+=i;

    long estimatedTime = System.currentTimeMillis() - startTime;
		System.out.println("Sum " + sum + ", elapsed = " + estimatedTime + " ms");
	
	}
}

