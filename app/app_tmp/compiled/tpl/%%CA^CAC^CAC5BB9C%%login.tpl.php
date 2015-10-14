<?php /* Smarty version 2.6.11, created on 2013-01-18 01:19:21
         compiled from ./atk/themes/steelblue/templates/login.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkconfig', './atk/themes/steelblue/templates/login.tpl', 1, false),array('function', 'atkthemeimg', './atk/themes/steelblue/templates/login.tpl', 3, false),array('function', 'atktext', './atk/themes/steelblue/templates/login.tpl', 23, false),)), $this); ?>
<?php echo smarty_function_atkconfig(array('var' => 'theme_login_logo','smartyvar' => 'login_logo'), $this);?>

<?php if (! $this->_tpl_vars['login_logo']):  echo smarty_function_atkconfig(array('var' => 'theme_logo','smartyvar' => 'login_logo'), $this); endif;  if (! $this->_tpl_vars['login_logo']):  ob_start();  echo smarty_function_atkthemeimg(array('0' => "login_logo.jpg"), $this); $this->_smarty_vars['capture']['default'] = ob_get_contents();  $this->assign('login_logo', ob_get_contents());ob_end_clean();  endif;  if (! $this->_tpl_vars['login_logo']):  ob_start();  echo smarty_function_atkthemeimg(array('0' => "logo.jpg"), $this); $this->_smarty_vars['capture']['default'] = ob_get_contents();  $this->assign('login_logo', ob_get_contents());ob_end_clean();  endif;  echo '
<style type="text/css">
body
{
'; ?>

  background: #f3f3f3;
	padding: 50px 24px 24px 24px;
<?php echo '
}
</style>
'; ?>



<div class="LoginHeader"><img src="<?php echo smarty_function_atkthemeimg(array('0' => "contentheaderright.jpg"), $this);?>
" align="right"></div>
<div id='loginform'>


<form action="<?php echo $this->_tpl_vars['formurl']; ?>
" method="post">
  <div id="loginform-title"><?php echo smarty_function_atktext(array('0' => 'login_form'), $this);?>
</div>
   <div id="loginform-content">
  <?php if (isset ( $this->_tpl_vars['auth_max_loginattempts_exceeded'] )): ?>
    <?php echo $this->_tpl_vars['auth_max_loginattempts_exceeded']; ?>

  <?php else: ?>
    <?php echo $this->_tpl_vars['atksessionformvars']; ?>

    <?php if (isset ( $this->_tpl_vars['auth_mismatch'] )): ?><span class="error"><?php echo $this->_tpl_vars['auth_mismatch']; ?>
</span><br><?php endif; ?>
    <?php if (isset ( $this->_tpl_vars['auth_account_locked'] )): ?><span class="error"><?php echo $this->_tpl_vars['auth_account_locked']; ?>
</span><br><?php endif; ?>
    <table cellpadding="0" cellspacing="0" border="0"><tr>
    <td class="loginformLabel"><?php echo smarty_function_atktext(array('0' => 'username'), $this);?>
:</td><td class="loginformField"><?php echo $this->_tpl_vars['userfield']; ?>
</td>
    </tr><tr>
    <td class="loginformLabel"><?php echo smarty_function_atktext(array('0' => 'password'), $this);?>
:</td><td class="loginformField"><input class="loginform" type="password" size="15" name="auth_pw" value=""></td>
    </tr><tr>
    <td class="loginformLabel"></td>
	<td>
    <input name="login" class="button atkdefaultbutton" type="submit" value="<?php echo smarty_function_atktext(array('0' => 'login'), $this);?>
">
	</td>
    <?php if ($this->_tpl_vars['auth_enablepasswordmailer']): ?>
	</tr><tr><td colspan="2"><br></br>
	<input name="login" class="button" type="submit" value="<?php echo smarty_function_atktext(array('0' => 'password_forgotten'), $this);?>
"></td>
	<?php endif; ?>
    </tr></table>
  <?php endif; ?>	
    <br><br><?php echo smarty_function_atktext(array('0' => 'login_guest_text'), $this);?>

  </div>
</form>
</div>
<div class="LoginFooter"><img src="<?php echo smarty_function_atkthemeimg(array('0' => "contentfooterright.jpg"), $this);?>
" align="right"></div>
