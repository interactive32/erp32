<?php /* Smarty version 2.6.11, created on 2013-01-18 01:20:47
         compiled from ./atk/themes/stillblue/templates/actionpage.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkmessages', './atk/themes/stillblue/templates/actionpage.tpl', 1, false),)), $this); ?>
<?php echo smarty_function_atkmessages(array(), $this);?>

<?php if (count ( $this->_tpl_vars['atkmessages'] )): ?>
<div class="atkmessages">
  <?php $_from = $this->_tpl_vars['atkmessages']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['message']):
?>
    <div class="atkmessages_<?php echo $this->_tpl_vars['message']['type']; ?>
"><?php echo $this->_tpl_vars['message']['message']; ?>
</div>
  <?php endforeach; endif; unset($_from); ?>
</div>
<?php endif; ?>
<div class="actionpageWrapper">

<table border="0" cellpadding="0" cellspacing="0">
<tr>

<!-- left sidebar -->
<?php if ($this->_tpl_vars['displayLeftSidebar']): ?>
<td valign="top" cellspacing="0" cellpadding="0">

<table class="panel" cellpadding="0" cellspacing="0" height="0" style='table-layout:fixed'> 
<tr>

  <tr> 
    <td class="panelHeader">
	<img src="./atk/themes/steelblue/images/contentheaderleftB.jpg" alt="" align="left">
	<img src="./atk/themes/steelblue/images/contentheaderright.jpg" alt="" align="right">
	</td> 
  </tr> 
  <tr> 
    <td class="panelContent"> 
      <div class="panelContent2"> 
		<?php echo $this->_tpl_vars['leftbar1']; ?>

      </div> 
	  
    </td> 
	
  </tr> 
  <tr> 
   <td class="panelFooter2">
    <img src="./atk/themes/steelblue/images/contentfoot2left.jpg" alt="" align="left">
    <img src="./atk/themes/steelblue/images/contentfoot2right.jpg" alt="" align="right">

   </td>
  </tr> 
  
</tr>


<tr height="10">
<!-- Blank space between panels -->
</tr>

<!-- terms of use -->
<tr>

  <tr> 
    <td class="panelHeader">
	<img src="./atk/themes/steelblue/images/contentheaderleftB.jpg" alt="" align="left">
	<img src="./atk/themes/steelblue/images/contentheaderright.jpg" alt="" align="right">
	</td> 
  </tr> 
  <tr> 
    <td class="panelContent"> 
      <div class="panelContent2"> 
		<?php echo $this->_tpl_vars['leftbar2']; ?>

      </div> 
	  
    </td> 
	
  </tr> 
  <tr> 
    <td class="panelFooter"><img src="./atk/themes/steelblue/images/contentfooterright.jpg" alt="" align="right"></td> 
  </tr> 
  
</tr>





</table>

</td>
<?php endif; ?>
<!-- left sidebar end -->

<td valign="top">
<?php $_from = $this->_tpl_vars['blocks']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['block']):
?>
  <?php echo $this->_tpl_vars['block']; ?>

<?php endforeach; endif; unset($_from); ?>
</td>
</tr>
</table>
<center>
<br><br>
<?php echo $this->_tpl_vars['actionfooter']; ?>

</center>

</div>