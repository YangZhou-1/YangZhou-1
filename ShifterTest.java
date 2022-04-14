import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

class ShifterTest {

	@Test
	void testShiftLeftNull() {
		int[] a = null;
		var s = new Shifter(a);
		int[] actual = s.shiftLeft();
		assertNull(actual);
	}

	@Test
	void testShiftRightNull() {
		int[] a = null;
		var s = new Shifter(a);
		int[] actual = s.shiftRight();
		assertNull(actual);
	}
	
	@Test
	void testShiftLeftOne() {
		int[] a = {1};
		var s = new Shifter(a);
		int[] actual = s.shiftLeft();
		int[] expected = {1};
		for(int i = 0; i < a.length; ++i) {
			assertEquals(expected[i], actual[i]);
		}
	}
	
	@Test
	void testShiftRightOne() {
		int[] a = {1};
		var s = new Shifter(a);
		int[] actual = s.shiftRight();
		int[] expected = {1};
		for(int i = 0; i < a.length; ++i) {
			assertEquals(expected[i], actual[i]);
		}
	}
	@Test
	void testShiftLeftTwo() {
		int[] a = {1, 2};
		// pivot 2, 1
		// shift 1, 2
		var s = new Shifter(a);
		int[] actual = s.shiftLeft();
		int[] expected = {1, 2};
		for(int i = 0; i < a.length; ++i) {
			assertEquals(expected[i], actual[i]);
		}
	}
	
	@Test
	void testPivot() {
		int[] a = {1, 2};
		// pivot 2, 1
		// shift 1, 2
		var s = new Shifter(a);
		int[] actual = s.pivot();
		int[] expected = {2, 1};
		for(int i = 0; i < a.length; ++i) {
			assertEquals(expected[i], actual[i]);
		}
	}
	
	@Test
	void testShift() {
		int[] a = {1, 2};
		// pivot 2, 1
		// shift 1, 2
		var s = new Shifter(a);
		int[] actual = s.pivot();
		int[] expected = {2, 1};
		for(int i = 0; i < a.length; ++i) {
			assertEquals(expected[i], actual[i]);
		}
	}

}
