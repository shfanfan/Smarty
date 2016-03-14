import {Component} from 'angular2/core';

@Component({
	selector: 'my-app',
	template: `
				<h1>smarty {{foo.name}} main page    {{cars|json}}  </h1>
				<ul  [style.color]="color" >
					<li *ngFor="#car of cars" > {{car}} 
					</li>
				</ul>
				<button (click)="btnClick('click')" (blur)="log('blur')">press me :)</button>
				<input [(ngModel)]="text" />
				<h3>{{text}}</h3>
`

})
export class AppComponent {
	cars = ['audi', 'bentli' ,'ford' ]
	foo ={ name:'aviv'}
	color="red";
	text = "text here"
	log = (msg){console.log(msg)}
	btnClick =  (msg){this.text += "."}
	
}