<?php

  useattrib("atkprofileattribute");

  
  class user_groups extends atkNode
  {

    function user_groups()
    {
      $this->atkNode("user_groups", NF_ADD_LINK);

      $this->add(new atkAttribute("id", AF_AUTOKEY));
	  $this->add(new atkAttribute("name", AF_OBLIGATORY|AF_UNIQUE|AF_SEARCHABLE));
	  $this->add(new atkProfileAttribute("accessrights",AF_HIDE_ADD|AF_BLANKLABEL));

      $this->setOrder("name");
      $this->setTable("admin_groups");
    }
	
    function descriptor_def()
    {
      return "[name]";
    }

  }

?>