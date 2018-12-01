package finalprojectB;


import junit.framework.TestCase;
import org.junit.Test;
import java.util.Random;
import static org.junit.Assert.*;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!





public class UrlValidatorTest extends TestCase {



	private static long allow_scheme = UrlValidator.ALLOW_ALL_SCHEMES;
	private UrlValidator urlVal = new UrlValidator(null, null, allow_scheme);
	

	
	public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   @Test
   public void testManualTestUrl()
   {
		//System.out.println("Hello!\n\n\n\n\n");
		

		
		//Test URLs		
		assertTrue(urlVal.isValid("http://www.google.com"));	//<-This worked!
		//Since "http://www.google.com" worked:
		assertTrue(urlVal.isValid("http://www.google.com:0")); //add a valid port <- FAILED!
		assertFalse(urlVal.isValid("http://www.google.com:-1")); //add an invalid port
		assertTrue(urlVal.isValid("http://www.google.com/test/")); //add a valid path <- FAILED!
		assertFalse(urlVal.isValid("http://www.google.com//../")); //add an invalid path
		assertTrue(urlVal.isValid("http://www.google.com/test/#a-fragment")); //add a valid fragment <- FAILED!
		assertFalse(urlVal.isValid("http://www.google.com:0/test/#<>`\"NOT FRAGMENT'")); //add an invalid fragment

		//Basic scheme and auth tests
		assertTrue(urlVal.isValid("ftp://www.google.com"));
		assertFalse(urlVal.isValid("30go--go://www.google.com"));
		assertTrue(urlVal.isValid("http://0.0.0.0"));
		assertFalse(urlVal.isValid("http://..1.2%.com"));

		//Other Tests		
		assertTrue(urlVal.isValid("file://test/")); //Special case file: allows for empty auth <- FAILED! Regular Expressions Are Missing
		assertTrue(urlVal.isValid("file://www.google.com:0")); //Test ability to catch trailing ":"
		assertTrue(urlVal.isValid("https://www.google.com")); // <- FAILED! Regular Expressions Are Missing
		assertTrue(urlVal.isValid("ftp://www.google.com/search?q=cats&ie=UTF-8")); // <- FAILED!
		assertTrue(urlVal.isValid("CAPS://www.google.com/")); // test capitalized scheme<- FAILED!
		assertTrue(urlVal.isValid("https://www.instagram.com/accounts/login/?next=%2Fbeavervip%2F&source=follow")); // <- FAILED!
		assertFalse(urlVal.isValid(""));
		assertFalse(urlVal.isValid("http://  www.  google.  com"));
		}
		
	//START PARTITION TESTING
   @Test
   public void testPartition1()
   {
	    //ALL VALID
		assertTrue(urlVal.isValid("http://www.google.com:0/test1/?action=edit&mode=up#a-fragment")); // <- FAILED!

   }
   
   @Test	
   public void testPartition2()
   {
	    //INVALID SCHEME
		assertFalse(urlVal.isValid("7X0-Z  Z//www.google.com:0/test1/?action=edit&mode=up#a-fragment"));

   }
   
   @Test	
   public void testPartition3()
   {
	    //INVALID AUTHORITY
		assertFalse(urlVal.isValid("http://.1.  2.3.4:0/test1/?action=edit&mode=up#a-fragment"));

   }

   @Test	
   public void testPartition4()
   {
	    //INVALID PORT
		assertFalse(urlVal.isValid("http://www.google.com:-65 a/test1/?action=edit&mode=up#a-fragment"));

   }

   @Test	
   public void testPartition5()
   {
	    //INVALID PATH
		assertFalse(urlVal.isValid("http://www.google.com:0/..  //?action=edit&mode=up#a-fragment"));

   }
   
   @Test	
   public void testPartition6()
   {
	    //INVALID QUERY
		assertFalse(urlVal.isValid("http://www.google.com:0/test1/<NOT>\"'  QUERY'\"#a-fragment"));

   } 
   
   @Test	
   public void testPartition7()
   {
	    //INVALID FRAGMENT
		assertFalse(urlVal.isValid("http://www.google.com:0/test1/?action=edit&mode=up#<>`\"NOT  FRAGMENT'"));

   }
   
   //END PARTITION TESTING
   
   
   public void testIsValid()
   {
	   
	   //StringBuilder testString = new StringBuilder();
	   boolean[] tester = new boolean[6];
	   ResultPair pair_holder = new ResultPair(null, true); 
	   String[] collection = new String [6];	//scheme, auth, port, path, query, fragment
	   
	   String[] success = new String [600000];
	   String[] fail = new String [30000];   
	   int success_count = 0;
	   int fail_count = 0;

	   int tetet = schemes.length;
	   assertTrue(tetet == 8);
	   
	   for(int i = 0; i < schemes.length; i++)
	   {

		   pair_holder = schemes[i];
		   collection[0] = pair_holder.item;
		   tester[0] = pair_holder.valid;
		   for(int j = 0; j < auth.length; j++)
		   {
			   pair_holder = auth[j];
			   collection[1] = pair_holder.item;
			   tester[1] = pair_holder.valid;
			   for(int k = 0; k < port.length; k++)
			   {
				   pair_holder = port[k];
				   collection[2] = pair_holder.item;
				   tester[2] = pair_holder.valid;
				   for(int m = 0; m < path.length; m++)
				   {
					   pair_holder = path[m];
					   collection[3] = pair_holder.item;
					   tester[3] = pair_holder.valid;
					   for(int n = 0; n < query.length; n++)
					   {
						   pair_holder = query[n];
						   collection[4] = pair_holder.item;
						   tester[4] = pair_holder.valid;
						   for(int p = 0; p < fragment.length; p++)
						   {
							   pair_holder = fragment[p];
							   collection[5] = pair_holder.item;
							   tester[5] = pair_holder.valid;
							   StringBuilder testString = new StringBuilder();
							   boolean final_test = true;
							   for(int r = 0; r < 6; r++)
							   {
								   testString.append(collection[r]);
								   if(tester[r] == false) {final_test = false;}
								   if(r == 5) // last pass in r loop
								   {
									   String url_input = testString.toString();
									   boolean url_test = urlVal.isValid(url_input);
									   if(url_test == final_test)//was valid
									   {
										   testString.append("     " + final_test);
										   testString.append(" " + url_test);
										   url_input = testString.toString();
										   success[success_count] = url_input;
										   success_count++;
									   }//end was valid
									   else
									   {
										   testString.append("     " + final_test);
										   testString.append(" " + url_test);
										   url_input = testString.toString();
										   fail[fail_count] = url_input;
										   fail_count++;
									   }//end else								   
									   
								   }//end if
								   
							   }//end r loop
						   }//end p loop
					   }//end n loop
				   }//end m loop
			   }//end k loop
		   }//end j loop  
	   }//end i loop
	   
	   //PRINT RESULTS SUCCESS
	   System.out.println("\n\nSUCCESS: \n");
	   for(int i = 0; i < success_count; i++)
	   {
		   System.out.println(success[i]);
	   }//End success for loop						
	   //PRINT RESULTS FAILURE

	   System.out.println("\n\nFAILURE: \n");
	   for(int i = 0; i < fail_count; i++)
	   {
		   System.out.println(fail[i]);
	   }//End success for loop	
	   
	   												
   }
   
	   //You can use this function for programming based testing
		ResultPair[] schemes = {new ResultPair("http://", true),
								new ResultPair("ftp://", true),
								new ResultPair("ftp+-.://", true),
								new ResultPair("CAPS:/", true),
								new ResultPair("3ht://", false),
								new ResultPair("https:", false),
								new ResultPair("ht  tps://", false),
								new ResultPair("", false)};	   
	   
		ResultPair[] auth = {new ResultPair("www.google.com", true),
                                new ResultPair("0.0.0.0", true),
								new ResultPair("GO.AU", true),
                                new ResultPair(".1.2.3.4", false),
								new ResultPair("go.a  u", true),
								new ResultPair("", false)};	   

		ResultPair[] port = {new ResultPair(":80", true),
								new ResultPair(":0",true),
								new ResultPair(": 4",false),
								new ResultPair(":!@#$%^&*()",false),
								new ResultPair(":65636",false),
								new ResultPair("", true)};   
								
								
 //		REGEX PATTERN FOR PATH
 //		^(/[-\\w:@&?=+,.~!*'%$_;\\(\\)]*)?$"								  
		ResultPair[] path = {new ResultPair("/test1", true),
								new ResultPair("/CAPS/", true),
								new ResultPair("/test1/test2/test3/", true),
								new ResultPair("/test:&?=.!~,*%+_$;'@()/", true),
								new ResultPair("/test  space/", false),		
								new ResultPair("/..", false),
								new ResultPair("/test//", false),
								new ResultPair("", true)};
								
		ResultPair[] query = {new ResultPair("?action=view", true),
								new ResultPair("?ACTION=EDIT", true),
								new ResultPair("@#adsasd-_+=`~/?|\"\"'';:{}[]<>,.()!@#$%^&*()", true),
								new ResultPair("NO WHITE SPACE", false),
								new ResultPair("", true)};							
								
		ResultPair[] fragment = {new ResultPair("#fragment", true),	
								new ResultPair("#FRAGMENT", true),			
								new ResultPair("#@#adsasd-_+=`~/?|\"\"'';:{}[]<>,.()!@#$%^&*()", true),
								new ResultPair("# WHITE SPACE  )", false),
								new ResultPair("", true)};


}
