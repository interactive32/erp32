<?php


class text_templates extends ia32Node
{

	function text_templates()
	{
		$this->atkNode("text_templates", NF_ADD_LINK);

		$this->add(new atkAttribute("id", AF_AUTOKEY));

		$this->add(new atkAttribute("name", AF_OBLIGATORY, $this->std_att_size));

		$this->add(new atkTextAttribute("text", array("cols"=>60,"rows"=>4), AF_OBLIGATORY));
		
		$this->setOrder("id");
		$this->setTable("text_templates");
	}


	function descriptor_def()
	{
		return "[name]";
	}


}

?>
