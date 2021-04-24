#https://www.docs4dev.com/docs/zh/python/3.7.2rc1/all/library-xmlrpc.client.html
import xmlrpc.client

with xmlrpc.client.ServerProxy("http://localhost:8000/") as proxy:
    print("3 is even: %s" % str(proxy.is_even(3)))
    print("100 is even: %s" % str(proxy.is_even(100)))