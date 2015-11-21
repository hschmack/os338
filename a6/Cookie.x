struct CookieRequest {
	int err; 		//return code
	int sister;		//1 for Tina, 2 for Judy
};

program COOKIE_JAR
{
  version COOKIE_JAR_VERSION
  {
    struct returnOne GET_COOKIE(struct CookieRequest) = 1;
  } = 1;
} = 0x2facaded;