from django import forms
from .models import User, Drink

class UserCreateForm(forms.ModelForm):
    class Meta:
        model = User
        fields = '__all__'

class LoginForm(forms.ModelForm):
    class Meta:
        model = User
        fields = ['user_id', 'password']

class DrinkEditCreate(forms.ModelForm):
    class Meta:
        model = Drink
        fields = '__all__'
    
class DrinkFilter(forms.Form):
    drink_name = forms.CharField(label='Drink Name', required=False)
    mg_caff = forms.IntegerField(label='Amount of Caffeine Less Than',required=False)
    g_sug = forms.IntegerField(label='Amount of Sugar Less Than',required=False)
