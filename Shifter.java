import java.util.Arrays;

public class Shifter {
	int[] values;
	
	private Shifter() {
		values = new int[10];
	}

	
	public Shifter(int[] a) {
		values = a;
	}

	public int[] shiftLeft() {
		if(values == null)	
			return null;
		if(values.length == 1)
			return Arrays.copyOf(values, values.length);
		
		
		int [] p = pivot();
		int [] sp = shift(p);
		return sp;
	}
	
	public int[] shift(int[] p) {
		int[] n = new int[values.length];
		for(int i = 0 ; i < values.length - 1; ++i) {
			n[i] = values[i+1];
		}
		n[values.length-1] = values[0];
		return n;
	}


	public int[] pivot() {
		// TODO revert to private, since this is our trade secret
		int[] n = new int[values.length];
		for(int i = 0; i < values.length; ++i) {
			n[i] = values[values.length- 1 - i];
		}
		
		return n;
	}


	public int[] shiftRight() {
		if(values == null)	
			return null;
		if(values.length == 1)
			return Arrays.copyOf(values, values.length);
		return new int[10];
	}
	
	
	
	

}
