#!/usr/bin/env python
#-*- coding: UTF-8 -*-
import os,sys
reload(sys)
sys.setdefaultencoding('utf8')
import getopt
import smtplib
from email.MIMEText import MIMEText
from email.MIMEMultipart import MIMEMultipart
from  subprocess import *

def sendqqmail(username,password,mailfrom,mailto,subject,content):
    gserver = 'smtp.qq.com'
    gport = 465

    try:
        msg = MIMEText(unicode(content).encode('utf-8'))
        msg['from'] = mailfrom
        msg['to'] = mailto
        msg['Reply-To'] = mailfrom
        msg['Subject'] = subject

        smtp = smtplib.SMTP_SSL(gserver, gport)
        smtp.set_debuglevel(0)
        smtp.ehlo()
        smtp.login(username,password)

        smtp.sendmail(mailfrom, mailto, msg.as_string())
        smtp.close()
    except Exception,err:
        print "Send mail failed. Error: %s" % err


def main():
    to=sys.argv[1]
    subject=sys.argv[2]
    content=sys.argv[3]
    sendqqmail('123456','gkhcvklnjyjsbgwic','123456@qq.com',to,subject,content)
if __name__ == "__main__":
    main()