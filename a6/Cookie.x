struct CookieRequest {
	int err;
	int sister;
};

program COOKIE_JAR
{
  version COOKIE_JAR_VERSION
  {
    struct CookieRequest GET_COOKIE(struct CookieRequest) = 1;
  } = 1;
} = 0x2facaded;