<?php /* Smarty version 2.6.11, created on 2013-01-18 01:20:50
         compiled from ./atk/themes/default/templates/list.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkmessages', './atk/themes/default/templates/list.tpl', 3, false),)), $this); ?>
<?php if (isset ( $this->_tpl_vars['formstart'] )):  echo $this->_tpl_vars['formstart'];  endif; ?>
<table border="0" cellspacing="0" cellpadding="2" width="100%">
  <?php echo smarty_function_atkmessages(array(), $this);?>

  <?php if (count ( $this->_tpl_vars['atkmessages'] )): ?>
    <tr>
      <td align="center" valign="top">
        <div class="atkmessages">
          <?php $_from = $this->_tpl_vars['atkmessages']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['message']):
?>
            <div class="atkmessages_<?php echo $this->_tpl_vars['message']['type']; ?>
"><?php echo $this->_tpl_vars['message']['message']; ?>
</div>
          <?php endforeach; endif; unset($_from); ?>
        </div>
        </div>
      </td
    </tr>        
  <?php endif; ?>  
  <?php if (( isset ( $this->_tpl_vars['header'] ) && ! empty ( $this->_tpl_vars['header'] ) )): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['header']; ?>
<br></td>
  </tr>
  <?php endif; ?>
  <?php if (( isset ( $this->_tpl_vars['index'] ) && ! empty ( $this->_tpl_vars['index'] ) )): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['index']; ?>
<br></td>
  </tr>
  <?php endif; ?>
  <?php if (( isset ( $this->_tpl_vars['navbar'] ) && ! empty ( $this->_tpl_vars['navbar'] ) )): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['navbar']; ?>
<br></td>
  </tr>
  <?php endif; ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['list']; ?>
<br></td>
  </tr>
  <?php if (( isset ( $this->_tpl_vars['navbar'] ) && ! empty ( $this->_tpl_vars['navbar'] ) )): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['navbar']; ?>
<br></td>
  </tr>
  <?php endif; ?>
  <?php if (( isset ( $this->_tpl_vars['footer'] ) && ! empty ( $this->_tpl_vars['footer'] ) )): ?>
  <tr>
    <td valign="top" align="left"><?php echo $this->_tpl_vars['footer']; ?>
<br></td>
  </tr>
  <?php endif; ?>
</table>
<?php if (isset ( $this->_tpl_vars['formstart'] )):  echo $this->_tpl_vars['formend'];  endif; ?>