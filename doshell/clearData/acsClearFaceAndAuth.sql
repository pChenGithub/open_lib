delete from face_img_info where state=5 and facestate=1;
update face_img_info set state=5,facestate=0 where state in (0,1);
update VersionInfo set Value=0 where Key='faceSignVer';
