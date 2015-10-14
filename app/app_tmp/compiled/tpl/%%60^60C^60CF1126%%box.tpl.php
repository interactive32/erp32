<?php /* Smarty version 2.6.11, created on 2013-01-18 01:20:47
         compiled from ./atk/themes/stillblue/templates/box.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkthemeimg', './atk/themes/stillblue/templates/box.tpl', 3, false),)), $this); ?>
<table class="panel" cellpadding="0" cellspacing="0">
  <tr>
    <td><div class="panelTitle"><span class="panelTitleText"><?php echo $this->_tpl_vars['title']; ?>
</span><div class="panelTitleRight"><img src="<?php echo smarty_function_atkthemeimg(array('0' => "tabRight.gif"), $this);?>
" alt="tabend"><div style="visibility: hidden" id="atkbusy"><img src="<?php echo smarty_function_atkthemeimg(array('0' => "spinner.gif"), $this);?>
" alt="Busy"/></div></div></div></td>
  </tr>
  <tr>
    <td class="panelHeader"><img src="<?php echo smarty_function_atkthemeimg(array('0' => "contentheaderright.jpg"), $this);?>
" alt="" align="right"></td>
  </tr>
  <tr>
    <td class="panelContent">
      <div class="panelContent2">
      <?php echo $this->_tpl_vars['content']; ?>

      </div>
    </td>
  </tr>
  <tr>
    <td class="panelFooter"><img src="<?php echo smarty_function_atkthemeimg(array('0' => "contentfooterright.jpg"), $this);?>
" alt="" align="right"></td>
  </tr>
</table>
<br class="clearfloat" />